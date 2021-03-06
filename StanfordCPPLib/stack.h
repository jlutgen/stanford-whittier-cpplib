/**
 * @file stack.h
 *
 * @brief
 * This file exports the Stack class, which implements
 * a collection that processes values in a last-in/first-out (LIFO) order.
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

#ifndef _stack_h
#define _stack_h

#include <sstream>
#include "hashcode.h"
#include "vector.h"

/**
 * @class Stack
 *
 * @brief This class models a linear structure called a <b><i>stack</i></b>
 * in which values are added and removed only from one end.
 *
 * This discipline gives rise to a last-in/first-out behavior (LIFO)
 * that is the defining feature of stacks.  The fundamental stack
 * operations are \ref push (add to top) and \ref pop
 * (remove from top).
 */

template <typename ValueType>
class Stack {

public:


/**
 * Creates a new empty stack.
 *
 * Sample usage:
 *
 *     Stack<ValueType> stack;
 */
   Stack();


/**
 * Frees any heap storage associated with this stack.
 */
   virtual ~Stack();


/**
 * Returns the number of values in this stack.
 *
 * Sample usage:
 *
 *     int n = stack.size();
 */
   int size() const;


/**
 * Returns \c true if this stack contains no elements.
 *
 * Sample usage:
 *
 *     if (stack.isEmpty()) ...
 */
   bool isEmpty() const;

/**
 * Pushes the specified value onto the top of this stack.
 * A synonym for the \ref push method.
 *
 * Sample usage:
 *
 *      stack.add(value);
 */
   void add(const ValueType& value);


/**
 * Removes all elements from this stack.
 *
 * Sample usage:
 *
 *     stack.clear();
 */
   void clear();


/**
 * Compares two stacks for equality.
 * Returns \c true if this stack contains exactly the same
 * values as the given other stack.
 * Identical in behavior to the \c == operator.
 *
 * Sample usage:
 *     if (stack.equals(stack2)) ...
 */
      bool equals(const Stack<ValueType>& s) const;


/**
 * Pushes the specified value onto this stack.
 *
 * Sample usage:
 *
 *     stack.push(value);
 */
   void push(ValueType value);


/**
 * Removes the top element from this stack and returns it.  This
 * method signals an error if this stack is empty.
 *
 * Sample usage:
 *
 *     ValueType top = stack.pop();
 */
   ValueType pop();


/**
 * Returns the top element from this stack, without removing
 * it.  This method signals an error if this stack is empty.  For
 * compatibility with the STL `stack` class, the Stanford-Whittier
 * `%Stack` class has a variant of this method called \ref top,
 * which returns the value by reference.
 *
 * Sample usage:
 *
 *     ValueType top = stack.peek();
 */
   ValueType peek() const;


/**
 * A variant of \ref peek, provided for compatibility
 * with the STL `stack` class.
 *
 * Returns a reference to the top element of this stack, without
 * removing it. This method signals an error if this stack is empty.
 *
 * Sample usage:
 *
 *     ValueType top = stack.top();
 */
   ValueType & top();


/**
 * Compares two stacks for equality.
 *
 * Sample usage:
 *
 *      if (stack == stack2) ...
 */
       bool operator ==(const Stack& s2) const;


/**
 * Compares two vectors for inequality.
 *
 * Sample usage:
 *
 *     if (stack != stack2) ...
 */
       bool operator !=(const Stack& s2) const;


/**
 * Returns a printable string representation of this stack.
 *
 * Sample usage:
 *
 *     string str = stack.toString();
 */
   std::string toString();


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Implementation notes: Stack data structure
 * ------------------------------------------
 * The easiest way to implement a stack is to store the elements in a
 * Vector.  Doing so means that the problems of dynamic memory allocation
 * and copy assignment are already solved by the implementation of the
 * underlying Vector class.
 */

   template <typename T>
   friend int hashCode(const Stack<T>& s);

private:
   Vector<ValueType> elements;

};

extern void error(std::string msg);

/*
 * Stack class implementation
 * --------------------------
 * The Stack is internally managed using a Vector.  This layered design
 * makes the implementation extremely simple, to the point that most
 * methods can be implemented in as single line.
 */

template <typename ValueType>
Stack<ValueType>::Stack() {
   /* Empty */
}

template <typename ValueType>
Stack<ValueType>::~Stack() {
   /* Empty */
}

template <typename ValueType>
int Stack<ValueType>::size() const {
   return elements.size();
}

template <typename ValueType>
bool Stack<ValueType>::isEmpty() const {
   return size() == 0;
}

template <typename ValueType>
void Stack<ValueType>::push(ValueType value) {
   elements.add(value);
}

template <typename ValueType>
ValueType Stack<ValueType>::pop() {
   if (isEmpty()) error("Stack::pop: Attempting to pop an empty stack");
   ValueType top = elements[elements.size() - 1];
   elements.remove(elements.size() - 1);
   return top;
}

template <typename ValueType>
ValueType Stack<ValueType>::peek() const {
   if (isEmpty()) error("Stack::peek: Attempting to peek at an empty stack");
   return elements.get(elements.size() - 1);
}

template <typename ValueType>
ValueType & Stack<ValueType>::top() {
   if (isEmpty()) error("Stack::top: Attempting to read top of an empty stack");
   return elements[elements.size() - 1];
}

template <typename ValueType>
void Stack<ValueType>::add(const ValueType& value) {
    push(value);
}

template <typename ValueType>
void Stack<ValueType>::clear() {
   elements.clear();
}

template <typename ValueType>
bool Stack<ValueType>::equals(const Stack<ValueType>& stack2) const {
    if (this == &stack2) {
        return true;
    }
    if (size() != stack2.size()) {
        return false;
    }
    for (int i = 0; i < size(); i++) {
        if (elements[i] != stack2.elements[i]) {
            return false;
        }
    }
    return true;
}

template <typename ValueType>
std::string Stack<ValueType>::toString() {
   std::ostringstream os;
   os << *this;
   return os.str();
}

template <typename ValueType>
bool Stack<ValueType>::operator ==(const Stack& stack2) const {
    return elements == stack2.elements;
}

template <typename ValueType>
bool Stack<ValueType>::operator !=(const Stack & stack2) const {
    return elements != stack2.elements;
}

/**
 * Overloads the `<<` operator so that it is able
 * to display stacks.
 *
 * Sample usage:
 *
 *     cout << stack;
 */
template <typename ValueType>
std::ostream & operator<<(std::ostream & os, const Stack<ValueType> & stack) {
   os << "{";
   Stack<ValueType> copy = stack;
   Stack<ValueType> reversed;
   while (!copy.isEmpty()) {
      reversed.push(copy.pop());
   }
   int len = stack.size();
   for (int i = 0; i < len; i++) {
      if (i > 0) os << ", ";
      writeGenericValue(os, reversed.pop(), true);
   }
   return os << "}";
}

template <typename ValueType>
std::istream & operator>>(std::istream & is, Stack<ValueType> & stack) {
   char ch;
   is >> ch;
   if (ch != '{') error("Stack::operator >>: Missing {");
   stack.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         ValueType value;
         readGenericValue(is, value);
         stack.push(value);
         is >> ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("Stack::operator >>: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

/*
 * Template hash function for stacks.
 * Requires the element type in the Stack to have a hashCode function.
 */
template <typename T>
int hashCode(const Stack<T>& s) {
    return hashCode(s.elements);
}

#endif
