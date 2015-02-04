/**
 * @file queue.h
 *
 * @brief
 * This file exports the Queue class, a collection
 * in which values are processed in a first-in/first-out
 * (FIFO) order.
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

#ifndef _queue_h
#define _queue_h

#include "hashcode.h"
#include "vector.h"


/**
 * This class models a linear structure called a <b><i>queue</i></b>
 * in which values are added at one end and removed from the other.
 * This discipline gives rise to a first-in/first-out behavior (FIFO)
 * that is the defining feature of queues.
 */
template <typename ValueType>
class Queue {

public:

/**
 * Creates a new empty queue.
 *
 * Sample usage:
 *
 *     Queue<ValueType> queue;
 */
   Queue();


/**
 * Frees any heap storage associated with this queue.
 */
   virtual ~Queue();


/**
 * Adds \em value to the end of the queue.
 * A synonym for the \ref enqueue method.
 *
 * Sample usage:
 *
 *      queue.add(value);
 */
   void add(const ValueType& value);


/**
 * Compares two queues for equality.
 * Returns \c true if this queue contains exactly the same
 * values as the given other queue.
 * Identical in behavior to the \c == operator.
 *
 * Sample usage:
 *
 *      if (queue.equals(queue2)) ...
 */
   bool equals(const Queue<ValueType>& queue2) const;


/**
 * Removes and returns the first item in the queue.
 * A synonym for the \ref dequeue method.
 *
 * Sample usage:
 *
 *      ValueType first = queue.remove();
 */
   ValueType remove();


/**
 * Returns the number of values in this queue.
 *
 * Sample usage:
 *
 *     int n = queue.size();
 */
   int size() const;


/**
 * Returns \c true if this queue contains no elements.
 *
 * Sample usage:
 *
 *     if (queue.isEmpty()) ...
 */
   bool isEmpty() const;


/**
 * Removes all elements from this queue.
 *
 * Sample usage:
 *
 *     queue.clear();
 */
   void clear();


/**
 * Adds a value to the end of this queue.
 *
 * Sample usage:
 *
 *     queue.enqueue(value);
 */

   void enqueue(ValueType value);


/**
 * Removes and returns the first item in this queue.
 *
 * Sample usage:
 *
 *     ValueType first = queue.dequeue();
 */
   ValueType dequeue();


/**
 * Returns the first value in this queue, without removing it.  For
 * compatibility with the STL `queue` class, the Stanford `%Queue` class
 * has a variant of this method called \ref front, which returns the
 * value by reference.
 *
 * Sample usage:
 *
 *     ValueType first = queue.peek();
 */
   ValueType peek() const;


/**
 * Returns the first value in this queue by reference.
 *
 * Sample usage:
 *
 *     ValueType first = queue.front();
 */
   ValueType & front();


/**
 * Returns the last value in this queue by reference.
 *
 * Sample usage:
 *
 *     ValueType last = queue.back();
 */
   ValueType & back();


/**
 * Returns a printable string representation of this queue.
 *
 * Sample usage:
 *
 *     string str = queue.toString();
 */
   std::string toString();


/**
 * Compares two queues for equality.
 *
 * Sample usage:
 *
 *      if (queue == queue2) ...
 */
    bool operator ==(const Queue& queue2) const;


/**
 * Compares two queues for inequality.
 *
 * Sample usage:
 *
 *     if (queue != queue2) ...
 */
    bool operator !=(const Queue& queue2) const;


    template <typename T>
    friend int hashCode(const Queue<T>& s);

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Implementation notes: Queue data structure
 * ------------------------------------------
 * The Queue class is implemented using a ring buffer.
 */

private:

/* Instance variables */

   Vector<ValueType> ringBuffer;
   int count;
   int capacity;
   int head;
   int tail;

/* Private functions */

   void expandRingBufferCapacity();

};

extern void error(std::string msg);

/*
 * Implementation notes: Queue data structure
 * ------------------------------------------
 * The array-based queue stores the elements in successive index
 * positions in a vector, just as a stack does.  What makes the
 * queue structure more complex is the need to avoid shifting
 * elements as the queue expands and contracts.  In the array
 * model, this goal is achieved by keeping track of both the
 * head and tail indices.  The tail index increases by one each
 * time an element is enqueued, and the head index increases by
 * one each time an element is dequeued.  Each index therefore
 * marches toward the end of the allocated vector and will
 * eventually reach the end.  Rather than allocate new memory,
 * this implementation lets each index wrap around back to the
 * beginning as if the ends of the array of elements were joined
 * to form a circle.  This representation is called a ring buffer.
 */

const int INITIAL_CAPACITY = 10;

/*
 * Implementation notes: Queue constructor
 * ---------------------------------------
 * The constructor must allocate the array storage for the queue
 * elements and initialize the fields of the object.
 */

template <typename ValueType>
Queue<ValueType>::Queue() {
   clear();
}

/*
 * Implementation notes: ~Queue destructor
 * ---------------------------------------
 * All of the dynamic memory is allocated in the Vector class,
 * so no work is required at this level.
 */

template <typename ValueType>
Queue<ValueType>::~Queue() {
   /* Empty */
}

template <typename ValueType>
void Queue<ValueType>::add(const ValueType& value) {
    enqueue(value);
}

template <typename ValueType>
int Queue<ValueType>::size() const {
   return count;
}

template <typename ValueType>
bool Queue<ValueType>::isEmpty() const {
   return count == 0;
}

template <typename ValueType>
void Queue<ValueType>::clear() {
   capacity = INITIAL_CAPACITY;
   ringBuffer = Vector<ValueType>(capacity);
   head = 0;
   tail = 0;
   count = 0;
}

template <typename ValueType>
void Queue<ValueType>::enqueue(ValueType value) {
   if (count >= capacity - 1) expandRingBufferCapacity();
   ringBuffer[tail] = value;
   tail = (tail + 1) % capacity;
   count++;
}

template <typename ValueType>
bool Queue<ValueType>::equals(const Queue<ValueType>& queue2) const {
    if (this == &queue2) {
        return true;
    }
    if (size() != queue2.size()) {
        return false;
    }
    Queue<ValueType> copy1 = *this;
    Queue<ValueType> copy2 = queue2;
    while (!copy1.isEmpty() && !copy2.isEmpty()) {
        if (!(copy1.dequeue() == copy2.dequeue())) {
            return false;
        }
    }
    return copy1.isEmpty() == copy2.isEmpty();
}

/*
 * Implementation notes: dequeue, peek
 * -----------------------------------
 * These methods must check for an empty queue and report an error
 * if there is no first element.
 */

template <typename ValueType>
ValueType Queue<ValueType>::dequeue() {
   if (count == 0) error("Queue::dequeue: Attempting to dequeue an empty queue");
   ValueType result = ringBuffer[head];
   head = (head + 1) % capacity;
   count--;
   return result;
}

template <typename ValueType>
ValueType Queue<ValueType>::peek() const {
   if (count == 0) error("Queue::peek: Attempting to peek at an empty queue");
   return ringBuffer.get(head);
}

template <typename ValueType>
ValueType Queue<ValueType>::remove() {
    // this isEmpty check is also done in dequeue(), but we repeat it
    // here so that the possible error message will be more descriptive.
    if (isEmpty()) {
        error("Queue::remove: Attempting to remove from an empty queue");
    }
    return dequeue();
}

template <typename ValueType>
ValueType & Queue<ValueType>::front() {
   if (count == 0) error("Queue::front: Attempting to read front of an empty queue");
   return ringBuffer[head];
}

template <typename ValueType>
ValueType & Queue<ValueType>::back() {
   if (count == 0) error("Queue::back: Attempting to read back of an empty queue");
   return ringBuffer[(tail + capacity - 1) % capacity];
}

/*
 * Implementation notes: expandRingBufferCapacity
 * ----------------------------------------------
 * This private method doubles the capacity of the ringBuffer vector.
 * Note that this implementation also shifts all the elements back to
 * the beginning of the vector.
 */

template <typename ValueType>
void Queue<ValueType>::expandRingBufferCapacity() {
   Vector<ValueType> copy = ringBuffer;
   ringBuffer = Vector<ValueType>(2 * capacity);
   for (int i = 0; i < count; i++) {
      ringBuffer[i] = copy[(head + i) % capacity];
   }
   head = 0;
   tail = count;
   capacity *= 2;
}

template <typename ValueType>
std::string Queue<ValueType>::toString() {
   ostringstream os;
   os << *this;
   return os.str();
}

template <typename ValueType>
bool Queue<ValueType>::operator ==(const Queue& queue2) const {
    return equals(queue2);
}

template <typename ValueType>
bool Queue<ValueType>::operator !=(const Queue& queue2) const {
    return !equals(queue2);
}

template <typename ValueType>
std::ostream & operator<<(std::ostream & os, const Queue<ValueType> & queue) {
   os << "{";
   Queue<ValueType> copy = queue;
   int len = queue.size();
   for (int i = 0; i < len; i++) {
      if (i > 0) os << ", ";
      writeGenericValue(os, copy.dequeue(), true);
   }
   return os << "}";
}

template <typename ValueType>
std::istream & operator>>(std::istream & is, Queue<ValueType> & queue) {
   char ch;
   is >> ch;
   if (ch != '{') error("Queue::operator >>: Missing {");
   queue.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         ValueType value;
         readGenericValue(is, value);
         queue.enqueue(value);
         is >> ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("Queue::operator >>: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

/*
 * Template hash function for queues.
 * Requires the element type in the queue to have a hashCode function.
 */
template <typename T>
int hashCode(const Queue<T>& q) {
    int code = HASH_SEED;
    for (int i = 0; i < q.count; i++) {
        code = HASH_MULTIPLIER * code + hashCode(q.ringBuffer[(q.head + i) % q.capacity]);
    }
    return int(code & HASH_MASK);
}

#endif
