/**
 * @file vector.h
 *
 * @brief
 * This file exports the Vector class, which provides an
 * efficient, safe, convenient replacement for the array type in C++.
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

#ifndef _vector_h
#define _vector_h

#include <iterator>
#include <iostream>
#include <sstream>
#include <string>
#include "foreach.h"
#include "hashcode.h"
#include "private/genericio.h"

/**
 * @class Vector
 *
 * @brief This class stores an ordered list of values similar to an array.
 *
 * It supports traditional array selection using square brackets, but
 * also supports inserting and deleting elements.  It is similar in
 * function to the STL <code>vector</code> type, but is simpler both
 * to use and to implement.
 */

template <typename ValueType>
class Vector {

public:

/**
 * Initializes a new vector.  The default constructor creates an
 * empty vector.  The second form creates an array with \em n
 * elements, each of which is initialized to \em value;
 * if \em value is missing, the elements are initialized
 * to the default value for the type.
 *
 * Sample usages:
 *
 *     Vector<ValueType> vec;
 *     Vector<ValueType> vec(n, value);
 */

   Vector();
   /** \_overload */
   explicit Vector(int n, ValueType value = ValueType());

/**
 * Frees any heap storage allocated by this vector.
 */

   virtual ~Vector();

/**
 * Returns the number of elements in this vector.
 *
 * Sample usage:
 *
 *     int nElems = vec.size();
 */

   int size() const;

/**
 * Returns <code>true</code> if this vector contains no elements.
 *
 * Sample usage:
 *
 *     if (vec.isEmpty()) ...
 */

   bool isEmpty() const;

/**
 * Removes all elements from this vector.
 *
 * Sample usage:
 *
 *     vec.clear();
 */


   void clear();

/**
 * Compares two vectors for equality.
 * Returns \c true if this vector contains exactly the same
 * values as the given other vector.
 * Identical in behavior to the \c == operator.
 * 
 * Sample usage:
 *
 *     if (vec.equals(vec2)) ...
 */
   bool equals(const Vector<ValueType>& v) const;


/**
 * Returns the element at the specified index in this vector.  This
 * method signals an error if the index is not in the array range.
 *
 * Sample usage:
 *
 *     ValueType val = vec.get(index);
 */

   const ValueType & get(int index) const;

/**
 * Replaces the element at the specified index in this vector with
 * a new value.  The previous value at that index is overwritten.
 * This method signals an error if the index is not in the array range.
 *
 * Sample usage:
 *
 *     vec.set(index, value);
 */

   void set(int index, const ValueType & value);

/**
 * Inserts the element into this vector before the specified index.
 * All subsequent elements are shifted one position to the right.  This
 * method signals an error if the index is outside the range from 0
 * up to and including the length of the vector.  To maintain
 * compatibility with older code, this method may also be called as
 * \ref insertAt;.
 *
 * Sample usage:
 *
 *     vec.insert(0, value);
 */

   void insert(int index, ValueType value);


/**
 * Alias for \ref insert.
 */
   void insertAt(int index, ValueType value);


/**
 * Removes the element at the specified index from this vector.  All
 * subsequent elements are shifted one position to the left.  This
 * method signals an error if the index is outside the array range.
 * To maintain compatibility with older code, this method may also
 * be called as \ref removeAt.
 *
 * Sample usage:
 *
 *     vec.remove(index);
 */
   void remove(int index);


/**
 * Alias for \ref remove.
 */
   void removeAt(int index);


/**
 * Adds a new value to the end of this vector.  To ensure compatibility
 * with the \c vector class in the Standard Template Library,
 * this method may also be called as \ref push_back.
 *
 * Sample usage:
 *
 *     vec.add(value);
 */
   void add(ValueType value);


/**
 * Alias for \ref add.
 */
   void push_back(ValueType value);


/**
 * Overloads the <code>[]</code> operator to select elements from this vector.
 * This extension enables the use of traditional array notation to
 * get or set individual elements.  This method signals an error if
 * the index is outside the array range.
 *
 * Sample usage:
 *
 *     vec[index]
 */
   ValueType & operator[](int index);
   const ValueType & operator[](int index) const;


/**
 * Concatenates two vectors.
 *
 * Sample usage:
 *
 *     v1 + v2
 */
   Vector operator+(const Vector & v2) const;


   /** \_overload */
   Vector & operator+=(const Vector & v2);
/**
 * Adds all of the elements from \em v2 (or the single
 * specified value) to \em v1.  As a convenience, the
 * \c %Vector class also overloads the comma operator so
 * that it is possible to initialize a vector like this:
 *
 * ~~~
 *    Vector<int> digits;
 *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
 * ~~~
 *
 * Sample usages:
 *
 *     v1 += v2;
 *     v1 += value;
 */
   Vector & operator+=(const ValueType & value);


/**
 * Compares two vectors for equality.
 *
 * Sample usage:
 *
 *      if (vec == vec2) ...
 */
    bool operator ==(const Vector& v2) const;


/**
 * Compares two vectors for inequality.
 *
 * Sample usage:
 *
 *     if (vec != vec2) ...
 */
    bool operator !=(const Vector& v2) const;


/**
 * Converts this vector to a printable string representation.
 *
 * Sample usage:
 *
 *     string str = vec.toString();
 */
   std::string toString();


/**
 * Calls the specified function on each element of this vector in
 * ascending index order.
 *
 * Sample usage:
 *
 *     vec.mapAll(fn);
 */
   void mapAll(void (*fn)(ValueType)) const;
   void mapAll(void (*fn)(const ValueType &)) const;

   template <typename FunctorType>
   void mapAll(FunctorType fn) const;

/*
 * Additional Vector operations
 * ----------------------------
 * In addition to the methods listed in this interface, the Vector
 * class supports the following operations:
 *
 *   - Stream I/O using the << and >> operators
 *   - Deep copying for the copy constructor and assignment operator
 *   - Iteration using the range-based for statement or STL iterators
 *
 * The iteration forms process the Vector in index order.
 */

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

/*
 * Implementation notes: Vector data structure
 * -------------------------------------------
 * The elements of the Vector are stored in a dynamic array of
 * the specified element type.  If the space in the array is ever
 * exhausted, the implementation doubles the array capacity.
 */

/* Instance variables */

   ValueType *elements;        /* A dynamic array of the elements   */
   int capacity;               /* The allocated size of the array   */
   int count;                  /* The number of elements in use     */

/* Private methods */

   void expandCapacity();
   void deepCopy(const Vector & src);

/*
 * Hidden features
 * ---------------
 * The remainder of this file consists of the code required to
 * support deep copying and iteration.  Including these methods
 * in the public interface would make that interface more
 * difficult to understand for the average client.
 */

public:

/*
 * Deep copying support
 * --------------------
 * This copy constructor and operator= are defined to make a deep copy,
 * making it possible to pass or return vectors by value and assign
 * from one vector to another.
 */

   Vector(const Vector & src);
   Vector & operator=(const Vector & src);

/*
 * Adds an element to the vector passed as the left-hand operatand.
 * This form makes it easier to initialize vectors in old versions of C++.
 */

   Vector & operator,(const ValueType & value);

/*
 * Iterator support
 * ----------------
 * The classes in the StanfordCPPLib collection implement input
 * iterators so that they work symmetrically with respect to the
 * corresponding STL classes.
 */

   class iterator :
      public std::iterator<std::random_access_iterator_tag, ValueType> {

   private:
      const Vector *vp;
      int index;

   public:

      iterator() {
         this->vp = NULL;
      }

      iterator(const iterator & it) {
         this->vp = it.vp;
         this->index = it.index;
      }

      iterator(const Vector *vp, int index) {
         this->vp = vp;
         this->index = index;
      }

      iterator & operator++() {
         index++;
         return *this;
      }

      iterator operator++(int) {
         iterator copy(*this);
         operator++();
         return copy;
      }

      iterator & operator--() {
         index--;
         return *this;
      }

      iterator operator--(int) {
         iterator copy(*this);
         operator--();
         return copy;
      }

      bool operator==(const iterator & rhs) {
         return vp == rhs.vp && index == rhs.index;
      }

      bool operator!=(const iterator & rhs) {
         return !(*this == rhs);
      }

      bool operator<(const iterator & rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Vector Iterator::operator <: Iterators are in different vectors");
         return index < rhs.index;
      }

      bool operator<=(const iterator & rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Vector Iterator::operator <=: Iterators are in different vectors");
         return index <= rhs.index;
      }

      bool operator>(const iterator & rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Vector Iterator::operator >: Iterators are in different vectors");
         return index > rhs.index;
      }

      bool operator>=(const iterator & rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Vector Iterator::operator >=: Iterators are in different vectors");
         return index >= rhs.index;
      }

      iterator operator+(const int & rhs) {
         return iterator(vp, index + rhs);
      }

      iterator operator+=(const int & rhs) {
         index += rhs;
         return *this;
      }

      iterator operator-(const int & rhs) {
         return iterator(vp, index - rhs);
      }

      iterator operator-=(const int & rhs) {
         index -= rhs;
         return *this;
      }

      int operator-(const iterator & rhs) {
         extern void error(std::string msg);
         if (vp != rhs.vp) error("Vector Iterator::operator -: Iterators are in different vectors");
         return index - rhs.index;
      }

      ValueType & operator*() {
         return vp->elements[index];
      }

      ValueType *operator->() {
         return &vp->elements[index];
      }

      ValueType & operator[](int k) {
         return vp->elements[index + k];
      }

   };

   iterator begin() const {
      return iterator(this, 0);
   }

   iterator end() const {
      return iterator(this, count);
   }

};

/* Implementation section */

extern void error(std::string msg);

/*
 * Implementation notes: Vector constructor and destructor
 * -------------------------------------------------------
 * The constructor allocates storage for the dynamic array
 * and initializes the other fields of the object.  The
 * destructor frees the memory used for the array.
 */

template <typename ValueType>
Vector<ValueType>::Vector() {
   count = capacity = 0;
   elements = NULL;
}

template <typename ValueType>
Vector<ValueType>::Vector(int n, ValueType value) {
   count = capacity = n;
   elements = (n == 0) ? NULL : new ValueType[n];
   for (int i = 0; i < n; i++) {
      elements[i] = value;
   }
}

template <typename ValueType>
Vector<ValueType>::~Vector() {
   if (elements != NULL) delete[] elements;
}

/*
 * Implementation notes: Vector methods
 * ------------------------------------
 * The basic Vector methods are straightforward and should require
 * no detailed documentation.
 */

template <typename ValueType>
int Vector<ValueType>::size() const {
   return count;
}

template <typename ValueType>
bool Vector<ValueType>::isEmpty() const {
   return count == 0;
}

template <typename ValueType>
void Vector<ValueType>::clear() {
   if (elements != NULL) delete[] elements;
   count = capacity = 0;
   elements = NULL;
}

template <typename ValueType>
bool Vector<ValueType>::equals(const Vector<ValueType>& v) const {
    if (this == &v) {
        return true;
    }
    if (size() != v.size()) {
        return false;
    }
    for (int i = 0, sz = size(); i < sz; i++) {
        if (get(i) != v.get(i)) {
            return false;
        }
    }
    return true;
}

template <typename ValueType>
const ValueType & Vector<ValueType>::get(int index) const {
    if (index < 0 || index >= count) {
      ostringstream out;
      out << "Vector::get: Index of " << index
          << " is outside of valid range of [0.." << (count-1) << "]";
      error(out.str());
    }
   return elements[index];
}

template <typename ValueType>
void Vector<ValueType>::set(int index, const ValueType & value) {
    if (index < 0 || index >= count) {
      ostringstream out;
      out << "Vector::set: Index of " << index
          << " is outside of valid range of [0.." << (count-1) << "]";
      error(out.str());
    }
   elements[index] = value;
}

/*
 * Implementation notes: insert, remove, add
 * -----------------------------------------
 * These methods must shift the existing elements in the array to
 * make room for a new element or to close up the space left by a
 * deleted one.
 */

template <typename ValueType>
void Vector<ValueType>::insert(int index, ValueType value) {
   if (count == capacity) expandCapacity();
   if (index < 0 || index > count) {
     ostringstream out;
     out << "Vector::insert: Index of " << index
         << " is outside of valid range of [0.." << count << "]";
     error(out.str());
   }
   for (int i = count; i > index; i--) {
      elements[i] = elements[i - 1];
   }
   elements[index] = value;
   count++;
}

template <typename ValueType>
void Vector<ValueType>::insertAt(int index, ValueType value) {
   insert(index, value);
}

template <typename ValueType>
void Vector<ValueType>::remove(int index) {
    if (index < 0 || index >= count) {
      ostringstream out;
      out << "Vector::remove: Index of " << index
          << " is outside of valid range of [0.." << (count-1) << "]";
      error(out.str());
    }
   for (int i = index; i < count - 1; i++) {
      elements[i] = elements[i + 1];
   }
   count--;
}

template <typename ValueType>
void Vector<ValueType>::removeAt(int index) {
   remove(index);
}

template <typename ValueType>
void Vector<ValueType>::add(ValueType value) {
   insert(count, value);
}

template <typename ValueType>
void Vector<ValueType>::push_back(ValueType value) {
   insert(count, value);
}

/*
 * Implementation notes: Vector selection
 * --------------------------------------
 * The following code implements traditional array selection using
 * square brackets for the index.
 */

template <typename ValueType>
ValueType & Vector<ValueType>::operator[](int index) {
    if (index < 0 || index >= count) {
      ostringstream out;
      out << "Vector::operator []: Selection index of " << index
          << " is outside of valid range of [0.." << (count-1) << "]";
      error(out.str());
    }
   return elements[index];
}
template <typename ValueType>
const ValueType & Vector<ValueType>::operator[](int index) const {
   if (index < 0 || index >= count) {
     ostringstream out;
     out << "Vector::operator []: Selection index of " << index
         << " is outside of valid range of [0.." << (count-1) << "]";
     error(out.str());
   }
   return elements[index];
}

template <typename ValueType>
Vector<ValueType> Vector<ValueType>::operator+(const Vector & v2) const {
   Vector<ValueType> vec = *this;
   foreach (ValueType value in v2) {
      vec.add(value);
   }
   return vec;
}

template <typename ValueType>
Vector<ValueType> & Vector<ValueType>::operator+=(const Vector & v2) {
   foreach (ValueType value in v2) {
      *this += value;
   }
   return *this;
}

template <typename ValueType>
Vector<ValueType> & Vector<ValueType>::operator+=(const ValueType & value) {
   this->add(value);
   return *this;
}

template <typename ValueType>
bool Vector<ValueType>::operator ==(const Vector& v2) const {
    return equals(v2);
}

template <typename ValueType>
bool Vector<ValueType>::operator !=(const Vector& v2) const {
    return !equals(v2);
}

template <typename ValueType>
std::string Vector<ValueType>::toString() {
   ostringstream os;
   os << *this;
   return os.str();
}

/*
 * Implementation notes: copy constructor and assignment operator
 * --------------------------------------------------------------
 * The constructor and assignment operators follow a standard paradigm,
 * as described in the associated textbook.
 */

template <typename ValueType>
Vector<ValueType>::Vector(const Vector & src) {
   deepCopy(src);
}

template <typename ValueType>
Vector<ValueType> & Vector<ValueType>::operator=(const Vector & src) {
   if (this != &src) {
      if (elements != NULL) delete[] elements;
      deepCopy(src);
   }
   return *this;
}

template <typename ValueType>
void Vector<ValueType>::deepCopy(const Vector & src) {
   count = capacity = src.count;
   elements = (capacity == 0) ? NULL : new ValueType[capacity];
   for (int i = 0; i < count; i++) {
      elements[i] = src.elements[i];
   }
}

/*
 * Implementation notes: The , operator
 * ------------------------------------
 * The comma operator works adding the right operand to the vector and
 * then returning the vector by reference so that it is set for the next
 * value in the chain.
 */

template <typename ValueType>
Vector<ValueType> & Vector<ValueType>::operator,(const ValueType & value) {
   this->add(value);
   return *this;
}

/*
 * Implementation notes: mapAll
 * ----------------------------
 * The various versions of the mapAll function apply the function or
 * function object to each element in ascending index order.
 */

template <typename ValueType>
void Vector<ValueType>::mapAll(void (*fn)(ValueType)) const {
   for (int i = 0; i < count; i++) {
      fn(elements[i]);
   }
}

template <typename ValueType>
void Vector<ValueType>::mapAll(void (*fn)(const ValueType &)) const {
   for (int i = 0; i < count; i++) {
      fn(elements[i]);
   }
}

template <typename ValueType>
template <typename FunctorType>
void Vector<ValueType>::mapAll(FunctorType fn) const {
   for (int i = 0; i < count; i++) {
      fn(elements[i]);
   }
}

/*
 * Implementation notes: expandCapacity
 * ------------------------------------
 * This function doubles the array capacity, copies the old elements
 * into the new array, and then frees the old one.
 */

template <typename ValueType>
void Vector<ValueType>::expandCapacity() {
   capacity = max(1, capacity * 2);
   ValueType *array = new ValueType[capacity];
   for (int i = 0; i < count; i++) {
      array[i] = elements[i];
   }
   if (elements != NULL) delete[] elements;
   elements = array;
}

/*
 * Implementation notes: << and >>
 * -------------------------------
 * The insertion and extraction operators use the template facilities in
 * strlib.h to read and write generic values in a way that treats strings
 * specially.
 */

/**
 * Overloads the `<<` operator so that it is able
 * to display vectors.
 *
 * Sample usage:
 *
 *     cout << vec;
 */
template <typename ValueType>
std::ostream & operator<<(std::ostream & os, const Vector<ValueType> & vec) {
   os << "{";
   int len = vec.size();
   for (int i = 0; i < len; i++) {
      if (i > 0) os << ", ";
      writeGenericValue(os, vec[i], true);
   }
   return os << "}";
}

template <typename ValueType>
std::istream & operator>>(std::istream & is, Vector<ValueType> & vec) {
   char ch;
   is >> ch;
   if (ch != '{') error("Vector::operator >>: Missing {");
   vec.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         ValueType value;
         readGenericValue(is, value);
         vec += value;
         is >> ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("Vector::operator >>: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

/*
 * Template hash function for vectors.
 * Requires the element type in the Vector to have a hashCode function.
 */
template <typename ValueType>
int hashCode(const Vector<ValueType>& v) {
    int code = HASH_SEED;
    for (ValueType element : v) {
        code = HASH_MULTIPLIER * code + hashCode(element);
    }
    return (code & HASH_MASK);
}

#endif
