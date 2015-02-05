/**
 * @file foreach.h
 *
 * @brief
 * This file defines the \em foreach keyword, which implements
 * a substitute for the range-based \em for loop from C++11.
 *
 * All iterable classes in the Stanford-Whittier libraries import this file, so
 * clients don't ordinarily need to do so explicitly.  This version of
 * \em foreach also supports C++ strings and arrays.
 *
 * The \em foreach statement steps through the elements in
 * a collection.  It works correctly with the collection classes in
 * both the Standard Template Library and the Stanford-Whittier C++ libraries,
 * but can also be used with C++ strings and statically initialized
 * arrays.
 *
 * The following code, for example, prints every element in the
 * string vector \em lines:
 *
 *     foreach (string str in lines) {
 *        cout << str << endl;
 *     }
 *
 *
 * Similarly, the following function calculates the sum of the character
 * codes in a string:
 *
 *     int sumCharacterCodes(string str) {
 *        int sum = 0;
 *        foreach (char ch in str) sum += ch;
 *        return sum;
 *     }
 *
 * As a simplification when iterating over maps, the \em foreach
 * macro iterates through the keys rather than the key/value pairs.
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

#ifndef _foreach_h
#define _foreach_h

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/// \cond foreach

#include <iterator>
#include <map>
#include <cstddef>
#include <cstring>

/* These #includes are for files that contain "in" as a token */

#include <ios>
#include <fstream>
#include <sstream>
#ifdef _WIN32
#  include <windows.h>
#endif
using namespace std;

/* Redefine the ios constants (one of which is "in") */

static const ios::openmode IOS_APP = ios::app;
static const ios::openmode IOS_ATE = ios::ate;
static const ios::openmode IOS_BINARY = ios::binary;
static const ios::openmode IOS_IN = ios::in;
static const ios::openmode IOS_OUT = ios::out;
static const ios::openmode IOS_TRUNC = ios::trunc;

/* Private implementation namespace */

namespace _fe {
   struct Range {
      virtual ~Range() { };
   };

   template <typename T>
   struct ArrayRange : Range {
      ArrayRange(const T *begin, const T *end) : iter(begin), end(end) { }
      const T *iter;
      const T *end;
   };

   template <typename CType>
   struct CRange : Range {
      CRange(const CType& c) :
         cont(c), iter(cont.begin()), end(cont.end()) { }
      CType cont;
      typename CType::iterator iter, end;
   };

   template <typename KT, typename VT, typename CT, typename AT>
   struct MapRange : Range {
      MapRange(const map<KT,VT,CT,AT> & c) :
         cont(c), iter(cont.begin()), end(cont.end()) { }
      map<KT,VT,CT,AT> cont;
      typename map<KT,VT,CT,AT>::iterator iter, end;
   };

/*
 * The State struct glues together all of these pieces and
 * stores all of the information throughout the loops.
 */

   struct State {
      State() : state(0), itr(NULL) { }
      ~State() { delete itr; }
      int state;
      Range *itr;
   };

/* General hook function */

   template <typename DowncastType, typename ValueType>
   ValueType HookImpl(State& fe) {
      DowncastType *ip = (DowncastType *) fe.itr;
      if (ip->iter == ip->end) {
         fe.state = 2;
         return ValueType();
      }
      fe.state = 1;
      ValueType vp = *ip->iter;     /* Subtle implementation note:    */
      ++ip->iter;                   /* Using *ip->iter++ here would   */
      return vp;                    /* require copying the iterator.  */
   }

/* Foreach implementation for containers */

   template <typename CType>
   CRange<CType> *Init(State & fe, const CType & collection) {
      fe.itr = new CRange<CType>(collection);
      return (CRange<CType>*) fe.itr;
   }

   template <typename CType>
   typename iterator_traits<typename CType::iterator>::value_type
   Hook(State & fe, CRange<CType> *) {
      return HookImpl<CRange<CType>,
         typename iterator_traits<typename CType::iterator>::value_type>(fe);
   }

/* For maps */

   template <typename K, typename V, typename C, typename A>
   MapRange<K,V,C,A> *Init(State & fe, const map<K,V,C,A> & collection) {
      fe.itr = new MapRange<K,V,C,A>(collection);
      return (MapRange<K,V,C,A>*) fe.itr;
   }

   template <typename DowncastType, typename ValueType>
   ValueType MapHookImpl(State & fe) {
      DowncastType *ip = (DowncastType *) fe.itr;
      if (ip->iter == ip->end) {
         fe.state = 2;
         return ValueType();
      }
      fe.state = 1;
      ValueType key = ip->iter->first;
      ++ip->iter;
      return key;
   }

   template <typename K, typename V, typename C, typename A>
   K Hook(State & fe, MapRange<K,V,C,A> *) {
      return MapHookImpl<MapRange<K,V,C,A>,K>(fe);
   }

/* For C strings */

   template <size_t n>
   ArrayRange<char> *Init(State & fe, char (&str)[n]) {
      fe.itr = new ArrayRange<char>(str, str + strlen(str));
      return (ArrayRange<char>*) fe.itr;
   }

   template <size_t n>
   ArrayRange<char> *Init(State & fe, const char (&str)[n]) {
      fe.itr = new ArrayRange<char>(str, str + strlen(str));
      return (ArrayRange<char>*) fe.itr;
   }

/* For arrays */

   template <typename T, size_t n>
   ArrayRange<T> *Init(State & fe, T (&arr)[n]) {
      fe.itr = new ArrayRange<T>(arr, arr + n);
      return (ArrayRange<T>*) fe.itr;
   }

   template <typename T, size_t n>
   ArrayRange<T> *Init(State & fe, const T (&arr)[n]) {
      fe.itr = new ArrayRange<T>(arr, arr + n);
      return (ArrayRange<T>*) fe.itr;
   }

   template <typename T>
   T Hook(State& fe, ArrayRange<T>*) {
      return HookImpl<ArrayRange<T>, T>(fe);
   }

}

/* The actual foreach and in macros */

#define foreach(arg) \
   for (_fe::State _fe; _fe.state < 2; ) \
      for (arg)); _fe.state++ == 1; _fe.state = 0)

#define in = _fe::Hook(_fe, _fe.state != 0 ? NULL : _fe::Init(_fe,

#endif

/// \endcond
