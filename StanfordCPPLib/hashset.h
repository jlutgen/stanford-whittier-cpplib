/**
 * @file hashset.h
 *
 * @brief
 * This file exports the HashSet class, which
 * implements an efficient abstraction for storing sets of distinct elements.
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

#ifndef _hashset_h
#define _hashset_h

#include <iostream>
#include "hashmap.h"
#include "vector.h"

/**
 * @class HashSet
 *
 * @brief This class implements an efficient abstraction for storing sets
 * of distinct elements.
 *
 * This class is identical to the Set class except that it uses a hash
 * table as its underlying representation.  The advantage of the
 * `%HashSet` class is that it operates in constant time, as opposed
 * to the <i>O</i>(log <i>N</i>) time for the Set class.  The disadvantage of
 * `%HashSet` is that iterators return the values in an unspecified order.
 */
template <typename ValueType>
class HashSet {

public:

/**
 * Initializes an empty set of the specified element type.
 * The type used for the elements must define
 * the <code>==</code> operator, and there must be a free function
 * with the following signature:
 *
 *      int hashCode(ValueType element);
 *
 * that returns a positive integer determined by the element.  The hashcode.h interface
 * exports \c hashCode functions for \c string and
 * the C++ primitive types. In addition, each of the Stanford-Whittier collection
 * classes (Vector, Stack, Queue, etc.) exports its own \c hashCode function.
 *
 * Sample usage:
 *
 *     HashSet<ValueType> set;
 */
   HashSet();


/**
 * Frees any heap storage associated with this set.
 */
   virtual ~HashSet();


/**
 * Returns \c true if this set contains exactly the same values
 * as the given other set.
 * Identical in behavior to the \c == operator.
 *
 * Sample usage:
 *
 *      if (set.equals(set2)) ...
 */
    bool equals(const HashSet<ValueType>& set2) const;


/**
 * Returns the number of elements in this set.
 *
 * Sample usage:
 *
 *     count = set.size();
 */
   int size() const;


/**
 * Returns \c true if this set contains no elements.
 *
 * Sample usage:
 *
 *     if (set.isEmpty()) ...
 */
   bool isEmpty() const;


/**
 * Adds an element to this set, if it was not already there.  For
 * compatibility with the STL `set` class, this method
 * is also exported as \ref insert.
 *
 * Sample usage:
 *
 *     set.add(value);
 */
   void add(const ValueType & value);


/**
 * An alias for \ref add.
 * Included for compatibility with the STL `set` class.
 */
   void insert(const ValueType & value);


/**
 * Removes an element from this set.  If \em value is not
 * contained in the set, no error is generated and the set
 * remains unchanged.
 *
 * Sample usage:
 *
 *     set.remove(value);
 */
   void remove(const ValueType & value);


/**
 * Returns \c true if the specified value is in this set.
 *
 * Sample usage:
 *
 *     if (set.contains(value)) ...
 */
   bool contains(const ValueType & value) const;


/**
 * Implements the subset relation on sets.  Returns
 * \c true if every element of this set is
 * contained in \em set2.
 *
 * Sample usage:
 *
 *     if (set.isSubsetOf(set2)) ...
 */
   bool isSubsetOf(const HashSet & set2) const;


/**
 * Removes all elements from this set.
 *
 * Sample usage:
 *
 *     set.clear();
 */
   void clear();


/**
 * Returns \c true if \em set1 and \em set2
 * contain the same elements.
 *
 * Sample usage:
 *
 *     set1 == set2
 */
   bool operator==(const HashSet & set2) const;


/**
 * Returns \c true if \em set1 and \em set2
 * are different.
 *
 * Sample usage:
 *
 *     set1 != set2
 */
   bool operator!=(const HashSet & set2) const;


/** \_overload */
   HashSet operator+(const HashSet & set2) const;
/**
 * Returns the union of sets \em set1 and \em set2, which
 * is the set of elements that are in at least one of the two sets.  The
 * right-hand operand can also be an element of the value type, in which
 * case the operator returns a new set formed by adding that element to \em set1.
 *
 * Sample usages:
 *
 *     set1 + set2
 *     set1 + element
 */  
   HashSet operator+(const ValueType & element) const;


/**
 * Returns the intersection of sets \em set1 and \em set2,
 * which is the set of all elements that are in both sets.
 *
 * Sample usage:
 *
 *     set1 * set2
 */
   HashSet operator*(const HashSet & set2) const;


/** \_overload */
   HashSet operator-(const HashSet & set2) const;
/**
 * Returns the difference of sets \em set1 and \em set2,
 * which is the set of all the elements that are in \em set1 but
 * not \em set2.  The right-hand operand can also be an
 * element of the value type, in which case the operator returns a new
 * set formed by removing that element from \em set1.
 *
 * Sample usages:
 *
 *     set1 - set2
 *     set1 - element
 */  
   HashSet operator-(const ValueType & element) const;


/** \_overload */
   HashSet & operator+=(const HashSet & set2);
/**
 * Adds all of the elements from \em set2 (or the single
 * specified value) to \em set1.  As a convenience, the
 * `%HashSet` package also overloads the comma operator so
 * that it is possible to initialize a set like this:
 *
 * ~~~
 *    HashSet<int> digits;
 *    digits += 0, 1, 2, 3, 4, 5, 6, 7, 8, 9;
 * ~~~
 *
 * Sample usages:
 *
 *     set1 += set2;
 *     set1 += value;
 */
   HashSet & operator+=(const ValueType & value);


/**
 * Removes any elements from \em set1 that are <i>not</i> in
 * \em set2.
 *
 * Sample usage:
 *
 *     set1 *= set2;
 */
   HashSet & operator*=(const HashSet & set2);


/** \_overload */
   HashSet & operator-=(const HashSet & set2);
/**
 * Removes the elements from \em set2 (or the single
 * specified value) from \em set1.  As a convenience, the
 * `%HashSet` package also overloads the comma operator so
 * that it is possible to remove multiple elements from a set
 * like this:
 *
 * ~~~
 *    digits -= 0, 2, 4, 6, 8;
 * ~~~
 *
 * which removes the values 0, 2, 4, 6, and 8 from the set
 * <code>digits</code>.
 *
 * Sample usages:
 *
 *     set1 -= set2;
 *     set1 -= value;
 */
   HashSet & operator-=(const ValueType & value);


/**
 * Returns the first value in this set. The ordering in a `%HashSet` is unspecified,
 * but the value returned by this method is the value that would be processed first
 * by the <code>foreach</code> macro.  If this set is empty, this method
 * signals an error.
 *
 * Sample usage:
 *
 *     ValueType value = set.first();
 */
   ValueType first() const;


/**
 * Returns a printable string representation of this set.
 *
 * Sample usage:
 *
 *     string str = set.toString();
 */
   std::string toString();


/**
 * Iterates through the elements of the set and calls <code>fn(value)</code>
 * for each one.  The values are processed in an unspecified order (but it is
 * the same order in which the <code>foreach</code> macro would process them).
 *
 * Sample usage:
 *
 *     set.mapAll(fn);
 */
   void mapAll(void (*fn)(ValueType)) const;
   void mapAll(void (*fn)(const ValueType &)) const;
   template <typename FunctorType>
   void mapAll(FunctorType fn) const;


/*
 * Additional HashSet operations
 * -----------------------------
 * In addition to the methods listed in this interface, the HashSet
 * class supports the following operations:
 *
 *   - Stream I/O using the << and >> operators
 *   - Deep copying for the copy constructor and assignment operator
 *   - Iteration using the range-based for statement and STL iterators
 *
 * The iteration forms process the HashSet in an unspecified order.
 */

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

   HashMap<ValueType,bool> map;        /* Map used to store the element     */
   bool removeFlag;                    /* Flag to differentiate += and -=   */

public:

/*
 * Hidden features
 * ---------------
 * The remainder of this file consists of the code required to
 * support the comma operator, deep copying, and iteration.
 * Including these methods in the public interface would make
 * that interface more difficult to understand for the average client.
 */

   HashSet & operator,(const ValueType & value) {
      if (this->removeFlag) {
         this->remove(value);
      } else {
         this->add(value);
      }
      return *this;
   }

/*
 * Iterator support
 * ----------------
 * The classes in the StanfordCPPLib collection implement input
 * iterators so that they work symmetrically with respect to the
 * corresponding STL classes.
 */

   class iterator : public std::iterator<std::input_iterator_tag,ValueType> {

   private:

      typename HashMap<ValueType,bool>::iterator mapit;

   public:

      iterator() {
         /* Empty */
      }

      iterator(typename HashMap<ValueType,bool>::iterator it) : mapit(it) {
         /* Empty */
      }

      iterator(const iterator & it) {
         mapit = it.mapit;
      }

      iterator & operator++() {
         ++mapit;
         return *this;
      }

      iterator operator++(int) {
         iterator copy(*this);
         operator++();
         return copy;
      }

      bool operator==(const iterator & rhs) {
         return mapit == rhs.mapit;
      }

      bool operator!=(const iterator & rhs) {
         return !(*this == rhs);
      }

      ValueType operator*() {
         return *mapit;
      }

      ValueType *operator->() {
         return mapit;
      }
   };

   iterator begin() const {
      return iterator(map.begin());
   }

   iterator end() const {
      return iterator(map.end());
   }

};

extern void error(std::string msg);

template <typename ValueType>
HashSet<ValueType>::HashSet() {
   /* Empty */
}

template <typename ValueType>
HashSet<ValueType>::~HashSet() {
   /* Empty */
}

template <typename ValueType>
int HashSet<ValueType>::size() const {
   return map.size();
}

template <typename ValueType>
bool HashSet<ValueType>::isEmpty() const {
   return map.isEmpty();
}

template <typename ValueType>
void HashSet<ValueType>::add(const ValueType & value) {
   map.put(value, true);
}

template <typename ValueType>
bool HashSet<ValueType>::equals(const HashSet<ValueType>& set2) const {
    // optimization: if literally same set, stop
    if (this == &set2) {
        return true;
    }

    if (size() != set2.size()) {
        return false;
    }

    return isSubsetOf(set2) && set2.isSubsetOf(*this);
}

template <typename ValueType>
void HashSet<ValueType>::insert(const ValueType & value) {
   map.put(value, true);
}

template <typename ValueType>
void HashSet<ValueType>::remove(const ValueType & value) {
   map.remove(value);
}

template <typename ValueType>
bool HashSet<ValueType>::contains(const ValueType & value) const {
   return map.containsKey(value);
}

template <typename ValueType>
void HashSet<ValueType>::clear() {
   map.clear();
}

template <typename ValueType>
bool HashSet<ValueType>::isSubsetOf(const HashSet & set2) const {
   iterator it = begin();
   iterator end = this->end();
   while (it != end) {
      if (!set2.map.containsKey(*it)) return false;
      ++it;
   }
   return true;
}

/*
 * Implementation notes: set operators
 * -----------------------------------
 * The implementations for the set operators use iteration to walk
 * over the elements in one or both sets.
 */

template <typename ValueType>
bool HashSet<ValueType>::operator ==(const HashSet& set2) const {
    return equals(set2);
}

template <typename ValueType>
bool HashSet<ValueType>::operator !=(const HashSet& set2) const {
    return !equals(set2);
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator+(const HashSet & set2) const {
   HashSet<ValueType> set = *this;
   for (ValueType value : set2) {
      set.add(value);
   }
   return set;
}

template <typename ValueType>
HashSet<ValueType>
HashSet<ValueType>::operator+(const ValueType & element) const {
   HashSet<ValueType> set = *this;
   set.add(element);
   return set;
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator*(const HashSet & set2) const {
   HashSet<ValueType> set;
   for (ValueType value : *this) {
      if (set2.map.containsKey(value)) set.add(value);
   }
   return set;
}

template <typename ValueType>
HashSet<ValueType> HashSet<ValueType>::operator-(const HashSet & set2) const {
   HashSet<ValueType> set;
   for (ValueType value : *this) {
      if (!set2.map.containsKey(value)) set.add(value);
   }
   return set;
}

template <typename ValueType>
HashSet<ValueType>
HashSet<ValueType>::operator-(const ValueType & element) const {
   HashSet<ValueType> set = *this;
   set.remove(element);
   return set;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator+=(const HashSet & set2) {
   for (ValueType value : set2) {
      this->add(value);
   }
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator+=(const ValueType & value) {
   this->add(value);
   this->removeFlag = false;
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator*=(const HashSet & set2) {
   Vector<ValueType> toRemove;
   for (ValueType value : *this) {
      if (!set2.map.containsKey(value)) toRemove.add(value);
   }
   for (ValueType value : toRemove) {
      this->remove(value);
   }
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator-=(const HashSet & set2) {
   Vector<ValueType> toRemove;
   for (ValueType value : *this) {
      if (set2.map.containsKey(value)) toRemove.add(value);
   }
   for (ValueType value : toRemove) {
      this->remove(value);
   }
   return *this;
}

template <typename ValueType>
HashSet<ValueType> & HashSet<ValueType>::operator-=(const ValueType & value) {
   this->remove(value);
   this->removeFlag = true;
   return *this;
}

template <typename ValueType>
ValueType HashSet<ValueType>::first() const {
   if (isEmpty()) error("HashSet::first: set is empty");
   return *begin();
}

template <typename ValueType>
std::string HashSet<ValueType>::toString() {
   ostringstream os;
   os << *this;
   return os.str();
}

template <typename ValueType>
void HashSet<ValueType>::mapAll(void (*fn)(ValueType)) const {
   map.mapAll(fn);
}

template <typename ValueType>
void HashSet<ValueType>::mapAll(void (*fn)(const ValueType &)) const {
   map.mapAll(fn);
}

template <typename ValueType>
template <typename FunctorType>
void HashSet<ValueType>::mapAll(FunctorType fn) const {
   map.mapAll(fn);
}

/**
 * Overloads the `<<` operator so that it is able
 * to display hash sets.
 *
 * Sample usage:
 *
 *     cout << set;
 */
template <typename ValueType>
std::ostream & operator<<(std::ostream & os, const HashSet<ValueType> & set) {
   os << "{";
   bool started = false;
   for (ValueType value : set) {
      if (started) os << ", ";
      writeGenericValue(os, value, true);
      started = true;
   }
   os << "}";
   return os;
}

template <typename ValueType>
std::istream & operator>>(std::istream & is, HashSet<ValueType> & set) {
   char ch;
   is >> ch;
   if (ch != '{') error("HashSet::operator >>: Missing {");
   set.clear();
   is >> ch;
   if (ch != '}') {
      is.unget();
      while (true) {
         ValueType value;
         readGenericValue(is, value);
         set += value;
         is >> ch;
         if (ch == '}') break;
         if (ch != ',') {
            error(std::string("HashSet::operator >>: Unexpected character ") + ch);
         }
      }
   }
   return is;
}

/*
 * Template hash function for hash sets.
 * Requires the element type in the HashSet to have a hashCode function.
 */
template <typename T>
int hashCode(const HashSet<T>& s) {
    int code = HASH_SEED;
    for (T n : s) {
        code = HASH_MULTIPLIER * code + hashCode(n);
    }
    return int(code & HASH_MASK);
}

#endif
