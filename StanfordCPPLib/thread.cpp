/*
 * File: thread.cpp
 * ----------------
 * This file implements the platform-independent parts of the thread package.
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

#include <iostream>
#include <sstream>
#include <string>
#include "thread.h"
#include "private/tplatform.h"
using namespace std;


Thread::Thread() {
    id = -1;
}

Thread::~Thread() {

   synchronized(getThreadRefCountLock()) {
        if (debug) cout << "*** synch: ~Thread(" << id << ")" << endl;
        if (debug) cout << "~Thread: @" << this << ", id " << id << endl;
        decThreadRefCountForPlatform(id); // JL
        if (debug) cout << "*** end synch: ~Thread(" << id << ")" << endl;
   }
}

string Thread::toString() {
   ostringstream stream;
   stream << "Thread" << id;
   return stream.str();
}

long Thread::getId() {
    return id;
}

Thread::Thread(long id, string s, bool initial) {
    this->id = id;
    if (!initial) {
        synchronized(getThreadRefCountLock()) {
          if (debug) cout << "*** synch: Thread(" << id << ")" << endl;
          if (debug) cout << "Thread::constructor(id) from " << s << ": @" << this << ", id " << id << endl;
            incThreadRefCountForPlatform(id, "constructor(id)");
          if (debug) cout << "*** end synch: Thread(" << id << ")" << endl;
        }
    }
}

// Copy constructor (JL)
Thread::Thread(const Thread& other) {
    id = other.id;

    synchronized(getThreadRefCountLock()) {
        if (debug) cout << "*** synch: copyConstructor(" << id << ")" << endl;
        if (debug) cout << "Thread::copyConstructor: @" << this << " <-- @" << &other << ", id " << id << endl;
        incThreadRefCountForPlatform(id, "copy constructor");
        if (debug) cout << "*** end synch: copyConstructor(" << id << ")" << endl;
    }
}

// Assignment operator (JL)
Thread& Thread::operator=(const Thread& other) {
    id = other.id;
    synchronized(getThreadRefCountLock()) {
         if (debug) cout << "*** synch: operator=(" << id << ")" << endl;
         if (debug) cout << "Thread::operator=: @" << this << " <-- @" << &other << ", id " << id << endl;
         incThreadRefCountForPlatform(id, "operator=");
         if (debug) cout << "*** end synch: operator=(" << id << ")" << endl;
    }
    return *this;
}

static void forkWithVoid(void *arg);

Thread fork(void (*fn)()) {
   Thread thread;
   StartWithVoid startup = { fn };
   long id = forkForPlatform(forkWithVoid, &startup);
   thread = Thread(id, "fork", true);
   return thread;
}

void join(Thread & thread) {
   joinForPlatform(thread.getId());
}

void yield() {
   yieldForPlatform();
}

Thread getCurrentThread() {  // TODO: clean this up
   Thread t;
   long id = getCurrentThreadForPlatform();
   t = Thread(id, "getCurrentThread");
//   cout << "getCurrentThread: returning Thread @" << &t << ", id " << t.getId() << endl;
   return t;
}

Lock::Lock() {
   id = initLockForPlatform();
}

Lock::~Lock() {
   decLockRefCountForPlatform(id);
}

void Lock::wait() {
   waitForPlatform(id);
}

void Lock::signal() {
   signalForPlatform(id);
}

// Copy constructor (JL)
Lock::Lock(const Lock& other) : id(other.id) {
    incLockRefCountForPlatform(id);
}

// Assignment operator (JL)
Lock& Lock::operator=(const Lock& other) {
    id = other.id;
    incLockRefCountForPlatform(id);
    return *this;
}

static void forkWithVoid(void *arg) {
   StartWithVoid *startup = (StartWithVoid *) arg;
   startup->fn();
}

Lock & getThreadRefCountLock() { // JL
   static Lock *lock = new Lock;
   return *lock;
}
