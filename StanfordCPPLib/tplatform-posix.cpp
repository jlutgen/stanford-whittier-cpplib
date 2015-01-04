/*
 * File: tplatform-posix.cpp
 * -------------------------
 * Implements the platform-specific code for threads using Posix threads.
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
#include <string>
#include <pthread.h>
#include <cstring>  // JL: for strerror
#include <sstream> // JL: temporary for debug msgs
#include "strlib.h"
#include "error.h"
#include "map.h"
#include "private/tplatform.h"
#include "thread.h"

using namespace std;

#define __macosx__ 1
#ifdef __macosx__
extern int sched_yield(void);
#define pthread_yield sched_yield
#endif

struct ThreadData {
   int id;
   pthread_t pid;
   bool terminated;
   void (*fn)(void *arg);
   void *arg;
   int refCount;
};

struct LockData {
   pthread_mutex_t mutex;
   pthread_cond_t condition;
   int depth;
   int owner;
   int refCount;
};

// added by JL - layout matches StartWithVoid and StartWithClientData from thread.h
struct StartInfo {
    void *fn;
    void *data;
};

// debug stuff
struct MemInfo {
    int refCount;
    string type;
};
// global map for debugging to keep track of
// dynamic memory
Map<void *, MemInfo> chunks;
void store(void *p, string type) {
    if (!chunks.containsKey(p)) {
        MemInfo info;
        info.refCount = 1;
        info.type = type;
        chunks[p] = info;
    } else {
        chunks[p].refCount++;
    }
}
void storeDel(void *p) {
    chunks[p].refCount--;
}

/* Constants */

#define MAX_THREAD_ID int(unsigned(-1) >> 1)
#define MAX_LOCK_ID   int(unsigned(-1) >> 1)

/* Private function prototypes */

static Map<int,ThreadData *> & getThreadDataMap();
static Map<int,LockData *> & getLockDataMap();
static int getNextFreeThread();
static int getNextFreeLock();
static pthread_key_t & getThreadDataKey();
static pthread_key_t *createThreadDataKey();
static void *startThread(void *arg);

/* Functions */

int forkForPlatform(void (*fn)(void *), void *arg) {
   int id = getNextFreeThread();
   ThreadData *tdp = new ThreadData;
   //store(tdp, "forkForPlatform tdp"); // DEBUG
   tdp->id = id; // JL
   tdp->terminated = false;
   tdp->refCount = 1;
   if (debug) printf("forkForPlatform(%d), setting refCount=1\n", id);
   tdp->fn = fn;
   //tdp->arg = arg;
   tdp->arg = new StartInfo; // JL
   //store(tdp->arg, "forkForPlatform tdp->arg"); // DEBUG
   *(StartInfo *)(tdp->arg) = *(StartInfo *) arg; // BUGFIX (JL): arg pointed to local struct in thread.h::fork that was sometimes
                                                  //   getting clobbered before new thread accessed it, so we copy the struct

   synchronized(getThreadRefCountLock()) {
      getThreadDataMap().put(id, tdp);
   }
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);  // BUGFIX (JL): was DETACHED, but need to be able to join, so...
   int osErr = pthread_create(&tdp->pid, &attr, startThread, tdp);
   pthread_attr_destroy(&attr);
   if (osErr != 0) error("fork: Can't create new thread");
   return id;
}

void incThreadRefCountForPlatform(int id, string s) {
   ThreadData *tdp = getThreadDataMap().get(id);
   if (tdp == NULL) {
     if (debug) cout << "incThreadRef from " << s <<  ", id " << id << " has null tdp. Doing nothing." << endl;
     return;
   }
   tdp->refCount++; // JL body
   if (debug) cout << "incThreadRef from " << s << ", id " << id << " refCount now " << tdp->refCount << endl;
}

void decThreadRefCountForPlatform(int id) { // JL body
    ThreadData *tdp = getThreadDataMap().get(id);
    if (tdp == NULL) {
        if (debug) cout << "decThreadRef: id " << id << " has null tdp. Doing nothing." << endl;
        return;
    }
    if (--(tdp->refCount) == 0) {
        if (debug) cout << "decThreadRef: id " << id << " refCount now " << tdp->refCount << endl;
        if (tdp->arg != NULL) {
           delete (StartInfo *) (tdp->arg);
           //storeDel(tdp->arg); // DEBUGGING
        }

        getThreadDataMap().remove(id);
        // storeDel(tdp); // DEBUGGING
        delete tdp;
    }
    else {
        if (debug) cout << "decThreadRef: id " << id << " refCount now " << tdp->refCount << endl;
    }
}

void joinForPlatform(int id) {
   pthread_t pid;
   synchronized(getThreadRefCountLock()) {
      ThreadData *tdp = getThreadDataMap().get(id);
      if (tdp == NULL) error(string("join: bad id: ") + integerToString(id));
      pid = tdp->pid;
   }
//   stringstream msg;
//   msg << "joinForPlatform: " << id << "->";
//   msg <<  tdp->pid << "\n";
//   cout << msg.str();
   int osErr = pthread_join(pid, NULL);
   if (osErr != 0) error(string("join: Can't join thread: ") + strerror(osErr)); // JL
}

int getCurrentThreadForPlatform() {
   ThreadData *tdp = (ThreadData *) pthread_getspecific(getThreadDataKey());
   if (tdp == NULL) {
      if (debug) cout << "getCurrentThreadForPlatform: tdp is NULL, creating id 0" << endl;
      tdp = new ThreadData;
      //store(tdp, "getCurrentThreadForPlatform tdp"); // DEBUG
      tdp->id = 0;
      tdp->arg = NULL; // JL
      pthread_setspecific(getThreadDataKey(), tdp);
   }
   return tdp->id;
}

void yieldForPlatform() {
   pthread_yield();
}

int initLockForPlatform() {
   int id = getNextFreeLock();
   LockData *ldp = new LockData;
   //store(ldp, "initLockForPlatform ldp");
   pthread_mutexattr_t attr;
   pthread_mutexattr_init(&attr);
   pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
   pthread_mutex_init(&ldp->mutex, &attr);
   pthread_mutexattr_destroy(&attr);
   pthread_cond_init(&ldp->condition, NULL);
   ldp->owner = -1;
   ldp->depth = 0;
   ldp->refCount = 1;
   getLockDataMap().put(id, ldp);
   return id;
}

void incLockRefCountForPlatform(int id) {
   getLockDataMap().get(id)->refCount++; // JL
}

void decLockRefCountForPlatform(int id) { // JL body
   LockData *ldp = getLockDataMap().get(id);
   if (--(ldp->refCount) == 0) {
       delete ldp;
       //if (id > 1) // not the global Lock
        //storeDel(ldp);
       getLockDataMap().remove(id);
   }
}

void lockForPlatform(int id) {
   LockData *ldp = getLockDataMap().get(id);
   pthread_mutex_lock(&ldp->mutex);
   if (ldp->depth++ == 0) {
        ldp->owner = getCurrentThreadForPlatform();
        if (debug) printf("LOCK #%d acquired by thread %d\n",  id, ldp->owner);
   }
}

void unlockForPlatform(int id) {
   LockData *ldp = getLockDataMap().get(id);
   if (--ldp->depth == 0) {
       if (debug) printf("UNLOCK #%d by thread %d\n", id, ldp->owner);
       ldp->owner = -1;
   }
   pthread_mutex_unlock(&ldp->mutex);
}

void waitForPlatform(int id) {
   LockData *ldp = getLockDataMap().get(id);
   pthread_cond_wait(&ldp->condition, &ldp->mutex);
}

void signalForPlatform(int id) {
   LockData *ldp = getLockDataMap().get(id);
   pthread_cond_broadcast(&ldp->condition);
}

/* Static functions */

static Map<int,ThreadData *> & getThreadDataMap() {
   static Map<int,ThreadData *> *mp = new Map<int,ThreadData *>();
   return *mp;
}

static Map<int,LockData *> & getLockDataMap() {
   static Map<int,LockData *> *mp = new Map<int,LockData *>();
   return *mp;
}

static int getNextFreeThread() {
   static int nextThread = 1;
   Map<int,ThreadData *> & map = getThreadDataMap();
   synchronized(getThreadRefCountLock()) {
      while (map.containsKey(nextThread)) {
         nextThread++;
         if (nextThread == MAX_THREAD_ID) nextThread = 1;
      }
   }
   return nextThread++;
}

static int getNextFreeLock() {
   static int nextLock = 1;
   Map<int,LockData *> & map = getLockDataMap();
   while (map.containsKey(nextLock)) {
      nextLock++;
      if (nextLock == MAX_LOCK_ID) nextLock = 1;
   }
   return nextLock++;
}

static pthread_key_t & getThreadDataKey() {
   static pthread_key_t *pkp = createThreadDataKey();
   return *pkp;
}

static pthread_key_t *createThreadDataKey() {
   pthread_key_t *pkp = new pthread_key_t;
   pthread_key_create(pkp, NULL);
   return pkp;
}

static void *startThread(void *arg) {
   ThreadData *tdp = (ThreadData *) arg;
   pthread_setspecific(getThreadDataKey(), tdp);
   try {
      tdp->fn(tdp->arg);
   } catch (ErrorException e) {
      cerr << "Error: " << e.getMessage() << endl;
      exit(1);
   }
   return NULL;
}

