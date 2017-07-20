/*
 * Copyright (c) 2000-2014 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of
 * SAMSUNG ELECTRONICS ("Confidential Information").
 * You shall not disclose such Confidential Information and shall
 * use it only in accordance with the terms of the license agreement
 * you entered into with SAMSUNG ELECTRONICS.
 * SAMSUNG make no representations or warranties about the suitability
 * of the software, either express or implied, including but not
 * limited to the implied warranties of merchantability, fitness for
 * a particular purpose, or non-infringement.
 * SAMSUNG shall not be liable for any damages suffered by licensee as
 * a result of using, modifying or distributing this software or its derivatives.

 */

#include <util/GHandler.h>

#ifdef USE_THREAD_POOL
#include "ThreadPool.h"
#endif

#include "util/Log.h"

#include <algorithm>

GHandler::GHandlerItem::GHandlerItem()
{
    post = NULL;
    message = NULL;
    time = 0;
    mAutoFree = false;
    mGroupId = -1;
}

#ifdef USE_THREAD_POOL
GHandler::GHandlerThread::GHandlerThread(GHandler *parent) : ThreadWorker()
#else
GHandler::GHandlerThread::GHandlerThread(GHandler *parent) :
        Thread()
#endif
{
    mParent = parent;
}

void GHandler::GHandlerThread::run()
{
    //_DBG("Thread is running.");
    g_main_loop_run(mParent->mLoop);
}

bool GHandler::__sort_queue(GHandlerItem a, GHandlerItem b)
{
    return (a.time < b.time);
}

GHandler::GHandler(bool main) :
        Handler(main)
{
    mInternalHandler = NULL;

    if (!mMain) {
        mContext = g_main_context_new();
        mLoop = g_main_loop_new(mContext, FALSE);
        g_main_context_unref(mContext);
        mThread = new GHandler::GHandlerThread(this);
#ifdef USE_THREAD_POOL
        ThreadPool::getInstance()->run(mThread);
#endif
    } else {
        mContext = g_main_context_default();
    }
    mHandlerRunning = FALSE;
    mDestroy = FALSE;
}

GHandler::GHandler(bool main, HandlerCallback *callback) :
        Handler(main, callback)
{
    mInternalHandler = NULL;

    if (!mMain) {
        mContext = g_main_context_new();
        mLoop = g_main_loop_new(mContext, FALSE);
        g_main_context_unref(mContext);
        mThread = new GHandler::GHandlerThread(this);
#ifdef USE_THREAD_POOL
        ThreadPool::getInstance()->run(mThread);
#endif
    } else {
        mContext = g_main_context_default();
    }
    mHandlerRunning = FALSE;
    mDestroy = FALSE;
}

GHandler::~GHandler()
{
    //_DBG("this handler is deleted = %p", this);

    mSync.lock();
    if (mInternalHandler) {
        g_source_destroy(mInternalHandler);
        g_source_unref(mInternalHandler);
        mInternalHandler = NULL;
    }

    for (std::vector<GHandlerItem>::iterator itr = mQueue.begin(); itr != mQueue.end();) {
        if (itr->post != NULL) {
            //_DBG("'worker' %p is removed", itr->post);
            itr = mQueue.erase(itr);
        } else if (itr->message != NULL) {
           // _DBG("'What' %d is removed", itr->message->what);
            delete itr->message;
            itr = mQueue.erase(itr);
        } else {
           // TryAssert(0, "Unknown queue item");
        }
    }
    mSync.unlock();

    if (!mMain) {
        g_main_loop_quit(mLoop);
        //_DBG("quited %p", this);
        delete mThread;
        g_main_loop_unref(mLoop);
    }

   // _DBG("end = %p", this);
}

void GHandler::selfDestroy()
{
    mSync.lock();
    mDestroy = TRUE;
    for (std::vector<GHandlerItem>::iterator itr = mQueue.begin(); itr != mQueue.end();) {
        if (itr->post != NULL) {
           // _DBG("'worker' %p is removed", itr->post);
            itr = mQueue.erase(itr);
        } else if (itr->message != NULL) {
           // _DBG("'What' %d is removed", itr->message->what);
            delete itr->message;
            itr = mQueue.erase(itr);
        } else {
            //TryAssert(0, "Unknown queue item");
        }
    }
    __start_handler();
    mSync.unlock();
}

bool GHandler::handleMessage(HandleMessage *msg)
{
    // default handler...
    return FALSE;
}

void GHandler::__start_handler() // call in mutex....
{
    if (mHandlerRunning == FALSE) {
        if (mInternalHandler) {
            g_source_destroy(mInternalHandler);
            g_source_unref(mInternalHandler);
            mInternalHandler = NULL;
        }

        mInternalHandler = g_idle_source_new();
        g_source_set_callback(mInternalHandler, __process_gloop, this, NULL);
        g_source_attach(mInternalHandler, mContext);
        mHandlerRunning = TRUE;
    }
}

gboolean GHandler::__process_gloop(gpointer data)
{
    gint64 currenttime = 0;
    GHandler *handler = (GHandler*) data;

    if (handler->mDestroy) {
        delete handler;
        return FALSE;
    }

    handler->mSync.lock();
    if (handler->mQueue.empty()) {
        //_DBG("Do not have any requested item handler = %p", handler);
        handler->mHandlerRunning = FALSE;
        g_source_unref(handler->mInternalHandler);
        handler->mInternalHandler = NULL;
        handler->mSync.unlock();
        return FALSE;
    }

    GHandlerItem item = handler->mQueue[0];
    currenttime = g_get_real_time() / 1000;
    if (item.time > currenttime + 10) // check neer 10msec
            {
        currenttime = item.time - currenttime - 10;
        g_source_unref(handler->mInternalHandler);
        handler->mInternalHandler = g_timeout_source_new(currenttime);
        g_source_set_callback(handler->mInternalHandler, __delay_gloop, handler,
        NULL);
        g_source_attach(handler->mInternalHandler, handler->mContext);
        handler->mHandlerRunning = FALSE;
        handler->mSync.unlock();
        return FALSE;
    }
    handler->mQueue.erase(handler->mQueue.begin()); // run
    handler->mSync.unlock();

    if (item.post) {
        item.post->run();
        if (item.mAutoFree) {
            delete item.post;
        }
    } else if (item.message) {
        HandleMessage *msg = item.message;
        if (handler->mCallback->handleMessage(msg)) {
            delete msg;
        }
    } else {
       // _ERR("post or message must exist value.");
    }

    return TRUE;
}

gboolean GHandler::__delay_gloop(gpointer data)
{
    GHandler *handler = (GHandler*) data;
    handler->mSync.lock();

    g_source_unref(handler->mInternalHandler);
    handler->mInternalHandler = NULL;
    if (handler->mHandlerRunning == FALSE) {
        handler->__start_handler();
    }
    handler->mSync.unlock();
    return FALSE;
}

bool GHandler::post(ThreadWorker *worker, bool bAutoFree, int nGroupId)
{
    mSync.lock();

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000);
    item.post = worker;
    item.mAutoFree = bAutoFree;
    item.mGroupId = nGroupId;
    mQueue.push_back(item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

bool GHandler::postHead(ThreadWorker *worker)
{
    if (mDestroy) {
        return FALSE;
    }

    mSync.lock();

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000);
    item.post = worker;
    mQueue.insert(mQueue.begin(), item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

bool GHandler::postTimeout(ThreadWorker *worker, TimeMS msec)
{
    if (mDestroy) {
        return FALSE;
    }

    mSync.lock();

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000) + msec;
    item.post = worker;
    mQueue.push_back(item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

bool GHandler::postTimeoutHead(ThreadWorker *worker, TimeMS msec)
{
    if (mDestroy) {
        return FALSE;
    }

    mSync.lock();

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000) + msec;
    item.post = worker;
    mQueue.insert(mQueue.begin(), item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

void GHandler::removeCallback(ThreadWorker *worker)
{
    if (mDestroy)
        return;
    mSync.lock();
    for (std::vector<GHandlerItem>::iterator itr = mQueue.begin(); itr != mQueue.end(); itr++) {
        if (itr->post != NULL && itr->post == worker) {
            mQueue.erase(itr);
           // _DBG("'worker' %p is removed", worker);
            break;
        }
    }
    mSync.unlock();
}

void GHandler::removeCallback(int nGroupId)
{
    if (mDestroy)
        return;
    mSync.lock();
    for (std::vector<GHandlerItem>::iterator itr = mQueue.begin(); itr != mQueue.end();) {
        if (itr->post != NULL && itr->mGroupId == nGroupId) {
            //_DBG("'worker' %p is removed", itr->post);
            if (itr->mAutoFree) {
                delete itr->post;
            }
            itr = mQueue.erase(itr);
        } else {
            itr++;
        }
    }
    mSync.unlock();
}

void GHandler::removeCallbackAll()
{
    if (mDestroy)
        return;
    mSync.lock();
    for (std::vector<GHandlerItem>::iterator itr = mQueue.begin(); itr != mQueue.end();) {
        if (itr->post != NULL) {
            //_DBG("'worker' %p is removed", itr->post);
            itr = mQueue.erase(itr);
        } else {
            itr++;
        }
    }
    mSync.unlock();
}

bool GHandler::sendMessage(HandleMessage *msg)
{
    if (mDestroy) {
        return FALSE;
    }

    mSync.lock();

    for (std::vector<GHandlerItem>::const_iterator itr = mQueue.begin(); itr != mQueue.end();
            itr++) {
        if (itr->message != NULL && itr->message->sender.compare(msg->sender) == 0
                && itr->message->what == msg->what) {
           // _ERR("'What' %d set twice by %s. Skip this request", msg->what, msg->sender.c_str());
            mSync.unlock();
            return FALSE;
        }
    }

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000);
    item.message = msg;
    mQueue.push_back(item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

bool GHandler::sendMessageHead(HandleMessage *msg)
{
    if (mDestroy) {
        return FALSE;
    }

    mSync.lock();

    for (std::vector<GHandlerItem>::const_iterator itr = mQueue.begin(); itr != mQueue.end();
            itr++) {
        if (itr->message != NULL && itr->message->sender.compare(msg->sender) == 0
                && itr->message->what == msg->what) {
            //_ERR("'What' %d set twice by %s. Skip this request", msg->what, msg->sender.c_str());
            mSync.unlock();
            return FALSE;
        }
    }

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000);
    item.message = msg;
    mQueue.insert(mQueue.begin(), item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

bool GHandler::sendMessageTimeout(HandleMessage *msg, TimeMS msec)
{
    if (mDestroy) {
        return FALSE;
    }

    mSync.lock();

    for (std::vector<GHandlerItem>::const_iterator itr = mQueue.begin(); itr != mQueue.end();
            itr++) {
        if (itr->message != NULL && itr->message->sender.compare(msg->sender) == 0
                && itr->message->what == msg->what) {
            //_ERR("'What' %d set twice by %s. Skip this request", msg->what, msg->sender.c_str());
            mSync.unlock();
            return FALSE;
        }
    }

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000) + msec;
    item.message = msg;
    mQueue.push_back(item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

bool GHandler::sendMessageTimeoutHead(HandleMessage *msg, TimeMS msec)
{
    if (mDestroy) {
        return FALSE;
    }

    mSync.lock();

    for (std::vector<GHandlerItem>::const_iterator itr = mQueue.begin(); itr != mQueue.end();
            itr++) {
        if (itr->message != NULL && itr->message->sender.compare(msg->sender) == 0
                && itr->message->what == msg->what) {
            //_ERR("'What' %d set twice by %s. Skip this request", msg->what, msg->sender.c_str());
            mSync.unlock();
            return FALSE;
        }
    }

    GHandlerItem item;
    item.time = (g_get_real_time() / 1000) + msec;
    item.message = msg;
    mQueue.insert(mQueue.begin(), item);
    std::sort(mQueue.begin(), mQueue.end(), __sort_queue);
    __start_handler();

    mSync.unlock();

    return TRUE;
}

void GHandler::removeMessage(int what)
{
    if (mDestroy)
        return;
    mSync.lock();
    for (std::vector<GHandlerItem>::iterator itr = mQueue.begin(); itr != mQueue.end(); itr++) {
        if (itr->message != NULL && itr->message->what == what) {
            delete itr->message;
            mQueue.erase(itr);
           // _DBG("'What' %d is removed", what);
            break;
        }
    }
    mSync.unlock();
}

void GHandler::removeMessageAll()
{
    if (mDestroy)
        return;
    mSync.lock();
    for (std::vector<GHandlerItem>::iterator itr = mQueue.begin(); itr != mQueue.end();) {
        if (itr->message != NULL) {
           // _DBG("'What' %d is removed", itr->message->what);
            delete itr->message;
            itr = mQueue.erase(itr);
            break;
        } else
            itr++;
    }
    mSync.unlock();
}

HandleMessage* GHandler::obtainMessage()
{
    HandleMessage* pMessage = new HandleMessage();
    pMessage->what = 0;
    pMessage->param1 = 0;
    pMessage->param2 = 0;
    pMessage->reply = NULL;
    pMessage->object = NULL;
    return pMessage;
}
HandleMessage* GHandler::obtainMessage(int what)
{
    HandleMessage* pMessage = new HandleMessage();
    pMessage->what = what;
    pMessage->param1 = 0;
    pMessage->param2 = 0;
    pMessage->reply = NULL;
    pMessage->object = NULL;

    return pMessage;
}

HandleMessage* GHandler::obtainMessage(int what, void* object)
{
    HandleMessage* pMessage = new HandleMessage();
    pMessage->what = what;
    pMessage->param1 = 0;
    pMessage->param2 = 0;
    pMessage->reply = NULL;
    pMessage->object = object;
    return pMessage;

}
HandleMessage* GHandler::obtainMessage(int what, int param1, int param2, void* object)
{
    HandleMessage* pMessage = new HandleMessage();
    pMessage->what = what;
    pMessage->param1 = param1;
    pMessage->param2 = param2;
    pMessage->reply = NULL;
    pMessage->object = object;
    return pMessage;
}
