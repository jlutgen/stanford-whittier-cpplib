/**
 * @file pqueue.h
 *
 * @brief
 * This file exports the PriorityQueue class, a
 * collection in which values are processed in priority order.
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

#ifndef _pqueue_h
#define _pqueue_h

#include "vector.h"

/**
 * This class models a structure called a <b><i>priority&nbsp;queue</i></b>
 * in which values are processed in order of priority.  As in conventional
 * English usage, lower priority numbers correspond to higher effective
 * priorities, so that a "priority 1" item takes precedence over a
 * "priority 2" item.
 */

template <typename ValueType>
class PriorityQueue {

public:

/**
 * Initializes a new priority queue, which is initially empty.
 *
 * Sample usage:
 *
 *     PriorityQueue<ValueType> pq;
 */
   PriorityQueue();


/**
 * Frees any heap storage associated with this priority queue.
 */
   virtual ~PriorityQueue();


/**
 * Returns the number of values in this priority queue.
 *
 * Sample usage:
 *
 *     int n = pq.size();
 */
   int size() const;


/**
 * Returns \c true if this priority queue contains no elements.
 *
 * Sample usage:
 *
 *     if (pq.isEmpty()) ...
 */
   bool isEmpty() const;


/**
 * Removes all elements from this priority queue.
 *
 * Sample usage:
 *
 *     pq.clear();
 */
   void clear();


/**
 * Adds \em value to this queue with the specified priority.
 * Lower priority numbers correspond to higher priorities, which
 * means that all "priority 1" elements are dequeued before any
 * "priority 2" elements.
 *
 * Sample usage:
 *
 *     pq.enqueue(value, priority);
 */
   void enqueue(ValueType value, double priority);


/**
 * Removes and returns the value in this queue with the highest priority.
 * If multiple values have the same priority, they are
 * dequeued in the same order in which they were enqueued.
 *
 * Sample usage:
 *
 *     ValueType first = pq.dequeue();
 */
   ValueType dequeue();


/**
 * Returns the value of highest priority in this queue, without
 * removing it.
 *
 * Sample usage:
 *
 *     ValueType first = pq.peek();
 */
   ValueType peek() const;


/**
 * Returns the priority of the first element in this queue, without
 * removing it.
 *
 * Sample usage:
 *
 *     double priority = pq.peekPriority();
 */
   double peekPriority() const;


/**
 * Returns the first value in this queue by reference, without
 * removing it.
 *
 * Sample usage:
 *
 *     ValueType first = pq.front();
 */
   ValueType & front();


/**
 * Returns the last value in this queue by reference, without
 * removing it.
 *
 * Sample usage:
 *
 *     ValueType last = pq.back();
 */
   ValueType & back();


/**
 * Returns a printable string representation of this priority queue.
 *
 * Sample usage:
 *
 *     string str = pq.toString();
 */
   std::string toString();


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Implementation notes: PriorityQueue data structure
 * --------------------------------------------------
 * The PriorityQueue class is implemented using a data structure called
 * a heap.
 */

private:

/* Type used for each heap entry */

   struct HeapEntry {
      ValueType value;
      double priority;
      long sequence;
   };

/* Instance variables */

   Vector<HeapEntry> heap;
   long enqueueCount;
   int backIndex;
   int count;
   int capacity;

/* Private function prototypes */

   void enqueueHeap(ValueType & value, double priority);
   ValueType dequeueHeap();
   bool takesPriority(int i1, int i2);
   void swapHeapEntries(int i1, int i2);

};

extern void error(std::string msg);

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue() {
   clear();
}

/*
 * Implementation notes: ~PriorityQueue destructor
 * -----------------------------------------------
 * All of the dynamic memory is allocated in the Vector class,
 * so no work is required at this level.
 */

template <typename ValueType>
PriorityQueue<ValueType>::~PriorityQueue() {
   /* Empty */
}

template <typename ValueType>
int PriorityQueue<ValueType>::size() const {
   return count;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::isEmpty() const {
   return count == 0;
}

template <typename ValueType>
void PriorityQueue<ValueType>::clear() {
   heap.clear();
   count = 0;
}

template <typename ValueType>
void PriorityQueue<ValueType>::enqueue(ValueType value, double priority) {
   if (count == heap.size()) heap.add(HeapEntry());
   int index = count++;
   heap[index].value = value;
   heap[index].priority = priority;
   heap[index].sequence = enqueueCount++;
   if (index == 0 || takesPriority(backIndex, index)) backIndex = index;
   while (index > 0) {
      int parent = (index - 1) / 2;
      if (takesPriority(parent, index)) break;
      swapHeapEntries(parent, index);
      index = parent;
   }
}

/*
 * Implementation notes: dequeue, peek, peekPriority
 * -------------------------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */

template <typename ValueType>
ValueType PriorityQueue<ValueType>::dequeue() {
   if (count == 0) error("PriorityQueue::dequeue: Attempting to dequeue an empty queue");
   count--;
   bool wasBack = (backIndex == count);
   ValueType value = heap[0].value;
   swapHeapEntries(0, count);
   int index = 0;
   while (true) {
      int left = 2 * index + 1;
      int right = 2 * index + 2;
      if (left >= count) break;
      int child = left;
      if (right < count && takesPriority(right, left)) child = right;
      if (takesPriority(index, child)) break;
      swapHeapEntries(index, child);
      index = child;
   }
   if (wasBack) backIndex = index;
   return value;
}

template <typename ValueType>
ValueType PriorityQueue<ValueType>::peek() const {
   if (count == 0) error("PriorityQueue::peek: Attempting to peek at an empty queue");
   return heap.get(0).value;
}

template <typename ValueType>
double PriorityQueue<ValueType>::peekPriority() const {
   if (count == 0) error("PriorityQueue::peekPriority: Attempting to peek at an empty queue");
   return heap.get(0).priority;
}

template <typename ValueType>
ValueType & PriorityQueue<ValueType>::front() {
   if (count == 0) error("PriorityQueue::front: Attempting to read front of an empty queue");
   return heap.get(0).value;
}

template <typename ValueType>
ValueType & PriorityQueue<ValueType>::back() {
   if (count == 0) error("PriorityQueue::back: Attempting to read back of an empty queue");
   return heap.get(backIndex).value;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::takesPriority(int i1, int i2) {
   if (heap[i1].priority < heap[i2].priority) return true;
   if (heap[i1].priority > heap[i2].priority) return false;
   return (heap[i1].sequence < heap[i2].sequence);
}

template <typename ValueType>
void PriorityQueue<ValueType>::swapHeapEntries(int i1, int i2) {
   HeapEntry entry = heap[i1];
   heap[i1] = heap[i2];
   heap[i2] = entry;
}

template <typename ValueType>
std::string PriorityQueue<ValueType>::toString() {
   ostringstream os;
   os << *this;
   return os.str();
}

template <typename ValueType>
std::ostream & operator<<(std::ostream & os,
                          const PriorityQueue<ValueType> & pq) {
   os << "{";
   PriorityQueue<ValueType> copy = pq;
   int len = pq.size();
   for (int i = 0; i < len; i++) {
      if (i > 0) os << ", ";
      os << copy.peekPriority() << ":";
      writeGenericValue(os, copy.dequeue(), true);
   }
   return os << "}";
}

template <typename ValueType>
std::istream & operator>>(std::istream & is, PriorityQueue<ValueType> & pq) {
   char ch;
   is >> ch;
   if (ch != '{') error("PriorityQueue::operator >>: Missing {");
   pq.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         double priority;
         is >> priority >> ch;
         if (ch != ':') error("PriorityQueue::operator >>: Missing colon after priority");
         ValueType value;
         readGenericValue(is, value);
         pq.enqueue(value, priority);
         is >> ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("PriorityQueue::operator >>: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

#endif
