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

#include <string>
#include <errno.h>
#include "Log.h"
#include "Thread.h"

extern "C"
{
    static void* __thread_run(void *param)
    {
        _DBG("Thread started.");
        ThreadWorker *worker = (ThreadWorker*) param;
        worker->run();
        return NULL;
    }
};

ThreadWorker::~ThreadWorker()
{

}

void Thread::_initializer(ThreadWorker *worker)
{
    int ret = 0;
    if (worker) {
        ret = pthread_create(&mThread, NULL, __thread_run, worker);
    } else {
        ret = pthread_create(&mThread, NULL, __thread_run, this);
    }

    if (ret == 0) {
        mRunFlag = 1;
    } else {
        std::string msg = "Failed to create thread";        
       _ERR("%s", msg.c_str());
    }
}

//sub class should implement this function
void Thread::run()
{

}

Thread::Thread()
{
    _initializer(NULL);
}

Thread::Thread(ThreadWorker *worker)
{
    _initializer(worker);
}

Thread::~Thread()
{
    if (pthread_join(mThread, NULL)) {
        std::string msg = "Failed to join thread";
        _ERR("%s", msg.c_str());
    }
}

bool Thread::isRunning()
{
    return mRunFlag;
}
