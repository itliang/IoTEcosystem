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

#ifndef __MUTEX_H__
#define __MUTEX_H__

#include <pthread.h>
#include <map>

class Mutex
{
private:
	pthread_mutex_t mutex;

public:
	Mutex();
	~Mutex();

	void lock();
	void trylock();
	void unlock();
};

///
/// SSimpleLock is a functor to wrap calling lock().  While this may seem
/// excessive, it permits a lot of flexibility in defining what it means
/// to acquire a lock
///
template <class Resource>
struct SSimpleLock
{
    typedef Resource resource_type;
    void operator()(resource_type& resource) const
    {
        resource.lock();
    }
};


///
/// SSimpleLock is a functor to wrap calling unlock().  While this may seem
/// excessive, it permits a lot of flexibility in defining what it means
/// to release a lock
///
template <class Resource>
struct SSimpleUnlock
{
    typedef Resource resource_type;
    void operator()(resource_type& resource) const
    {
        resource.unlock();
    }
};


///
/// class CGuard<> implements a templatized "resource acquisition is
/// initialization" (RAII) locking guard.
///
/// This guard is useful for locking resources in an exception-safe manner.
/// The classic use of this is to lock a mutex within a C++ scope, as follows:
///
/// void SomeFunction()
/// {
///     CGuard<CMutex> GUARD(some_mutex);
///     [...perform some thread-safe operations...]
/// }
///
/// If an exception is thrown during the performance of any operations while
/// the guard is held, the guarantee of C++ stack-unwinding will force
/// the guard's destructor to release whatever resources were acquired.
///
///

enum EEmptyGuard {
    eEmptyGuard
};

template <class Resource,
          class Lock = SSimpleLock<Resource>,
          class Unlock = SSimpleUnlock<Resource> >
class CGuard
{
public:
    typedef Resource resource_type;
    typedef resource_type* resource_ptr;
    typedef Lock lock_type;
    typedef Unlock unlock_type;
    typedef CGuard<Resource, Lock, Unlock> TThisType;

    explicit CGuard(EEmptyGuard /*empty*/)
        {
        }

    /// This constructor locks the resource passed.
    explicit CGuard(const resource_type& resource)
        {
            Guard(resource);
        }

    explicit CGuard(EEmptyGuard /*empty*/,
                    const lock_type& lock,
                    const unlock_type& unlock = unlock_type())
        : m_Data(lock, std::map<unlock_type, resource_ptr>(unlock, 0))
        {
        }

    /// This constructor locks the resource passed.
    explicit CGuard(const resource_type& resource,
                    const lock_type& lock,
                    const unlock_type& unlock = unlock_type())
        : m_Data(lock, std::map<unlock_type, resource_ptr>(unlock, 0))
        {
            Guard(resource);
        }

    /// Destructor releases the resource.
    ~CGuard()
        {
            try {
                Release();
            } catch(std::exception& ) {
                // catch and ignore std exceptions in destructor
            }
        }

    /// Manually force the resource to be released.
    void Release()
        {
            if ( GetResource() ) {
                GetUnlock()(*GetResource());
                GetResource() = 0;
            }
        }

    /// Manually force the guard to protect some other resource.
    void Guard(resource_type& resource)
        {
            Release();
            GetLock()(resource);
            GetResource() = &resource;
        }

protected:
    resource_ptr& GetResource(void)
        {
            return m_Data.begin()->second->second;
        }
    lock_type& GetLock(void)
        {
            return m_Data.begin()->first;
        }
    unlock_type& GetUnlock(void)
        {
            return m_Data.begin()->second->first;
        }

private:
    /// Maintain a pointer to the original resource that is being guarded
    std::map<lock_type,
                     std::map<unlock_type,
                                      resource_ptr> >  m_Data;
};

typedef CGuard<Mutex> MutexGuard;



#endif //__MUTEX_H__