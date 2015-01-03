/**
 * @file thread.h
 *
 * This file exports a simple, platform-independent thread abstraction,
 * along with simple tools for concurrency control.
 *
 * For convenience, this file defines a \c synchronized macro that
 * takes a Lock argument and is used to mark a critical section of code protected
 * by the specified lock.  The general strategy for using this facility
 * is shown in the following paradigmatic pattern:
 *
 * ~~~
 *    synchronized (lock) {
 *       ... statements in the critical section ...
 *    }
 * ~~~
 */

/*************************************************************************/
/* Stanford Portable Library                                             */
/* Copyright (c) 2014 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

#ifndef _thread_h
#define _thread_h

#include <string>

/* Forward definition */

class Lock;


/**
 * \class Thread
 *
 * This class encapsulates a lightweight process running in the same address
 * space as the creator.  The class itself is opaque and is manipulated by
 * top-level functions as illustrated in the following paradigm:
 *
 * ~~~
 *    Thread child = fork(fn);
 *    ... code for the parent thread ...
 *    join(child);
 * ~~~
 *
 * This code calls <code>fn</code> so that it runs in parallel with the
 * parent code.
 */
class Thread {

public:

/**
 * Creates an inactive thread variable that will typically be overwritten
 * by the result of a \ref fork call.
 *
 * Sample usage:
 * ~~~
 * Thread thread;
 * ~~~
 */
   Thread();


/**
 * Frees any dynamic storage associated with this thread.
 */
   virtual ~Thread();


/**
 * Returns a printable string representation of this thread.
 *
 * Sample usage:
 * ~~~
 * string str = thread.toString();
 * ~~~
 */
   std::string toString();


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in this class is logically part  */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

   long id;   /* id linking this thread to the platform-specific data */

};


/** \_overload */
Thread fork(void (*fn)());
/**
 * Creates a child thread that calls \em fn in an address space
 * shared with the current thread.  The second form makes it possible to
 * pass an argument to \em fn, which may be of any type.
 *
 * Sample usages:
 * ~~~
 * Thread child = fork(fn);
 * Thread child = fork(fn, data);
 * ~~~
 */
template <typename ClientType>
Thread fork(void (*fn)(ClientType & data), ClientType & data);


/**
 * Waits for the specified thread to finish before proceeding.
 *
 * Sample usage:
 * ~~~
 * join(thread);
 * ~~~
 */
void join(Thread & thread);


/**
 * Yields the processor to allow another thread to run.
 *
 * Sample usage:
 * ~~~
 * yield();
 * ~~~
 */
void yield();


/**
 * Returns the currently executing thread.
 *
 * Sample usage:
 * ~~~
 * Thread self = getCurrentThread();
 * ~~~
 */
Thread getCurrentThread();


/**
 * \class Lock
 *
 * This class represents a simple lock used to control concurrency.  The
 * usual strategy for using locks is to use the \link thread.h synchronized \endlink
 * macro.
 */
class Lock {

public:

/**
 * Initializes a lock, which is initially in the unlocked state.
 *
 * Sample usage:
 * ~~~
 * Lock lock;
 * ~~~
 */
   Lock();


/**
 * Frees any heap storage associated with this lock.
 */
   ~Lock();


/**
 * Waits for some other thread to call \ref signal on this lock.
 * This call requires that the lock be held by the calling thread.
 * The effect of this method is to release the lock
 * and then wait until the desired \c signal operation occurs,
 * at which point the lock is reacquired and control returns from the
 * \c wait call.  This method is typically
 * used inside a critical section containing a \c while loop
 * to check for a specific condition.  The standard paradigm for using
 * this method looks like this:
 *
 * ~~~
 *    synchronized (lock) {
 *       while (conditional test) {
 *          lock.wait();
 *       }
 *       ... code to manipulate the locked resource ...
 *    }
 * ~~~
 *
 * Sample usage:
 * ~~~
 * lock.wait();
 * ~~~
 */
   void wait();


/**
 * Signals all threads waiting on this lock so that they wake up and
 * recheck the corresponding condition.
 *
 * Sample usage:
 * ~~~
 * lock.signal();
 * ~~~
 */
   void signal();


/**********************************************************************/
/* Note: Everything below this point in this class is logically part  */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

   long id;     /* id linking this lock to the platform-specific data */

   friend class Lock_State;

};



void lockForPlatform(int id);
void unlockForPlatform(int id);

class Lock_State {
public:

   Lock_State(Lock & lock) {
      lp = &lock;
      finished = false;
   }

   bool advance() {
      if (finished) {
         unlockForPlatform(lp->id);
         return false;
      } else {
         finished = true;
         lockForPlatform(lp->id);
         return true;
      }
   }

private:
   Lock *lp;
   bool finished;

};

/*
 * Defines a critical section protected by the specified lock.  The
 * general strategy for using this facility is shown in the following
 * paradigmatic pattern:
 *
 * ~~~
 *    synchronized (lock) {
 *       ... statements in the critical section ...
 *    }
 * ~~~
 */

#define synchronized(lock) for (Lock_State ls(lock) ; ls.advance(); )

int forkForPlatform(void (*fn)(void *), void *dp);

struct StartWithVoid {
   void (*fn)();
};

template <typename ClientType>
struct StartWithClientData {
   void (*fn)(ClientType & data);
   ClientType *dp;
};

template <typename ClientType>
static void forkWithClientData(void *arg) {
   StartWithClientData<ClientType> *startup =
      (StartWithClientData<ClientType> *) arg;
   startup->fn(*startup->dp);
}

template <typename ClientType>
Thread fork(void (*fn)(ClientType & data), ClientType & data) {
   StartWithClientData<ClientType> startup = { fn, &data };
   Thread thread;
   thread.id = forkForPlatform(forkWithClientData<ClientType>, &startup);
   return thread;
}

#endif
