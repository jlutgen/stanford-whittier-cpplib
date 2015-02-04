/**
 * @file lexicon.h
 *
 * @brief
 * This file exports the Lexicon class, which is a
 * compact structure for storing a list of words.
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

#ifndef _lexicon_h
#define _lexicon_h

#include <string>
#include <cctype>
#include "foreach.h"
#include "lexicon.h"
#include "set.h"
#include "stack.h"

/**
 * This class is used to represent a <b><i>lexicon,</i></b> or word list.
 * The main difference between a lexicon and a dictionary is that
 * a lexicon does not provide any mechanism for storing definitions;
 * the lexicon contains only words, with no associated information.
 * It is therefore similar to a set of strings, but with a more
 * space-efficient internal representation.  This class
 * supports efficient lookup operations for words and prefixes.
 *
 * As an example of the use of the Lexicon class, the
 * following program lists all the two-letter words in the lexicon
 * stored in the file <code>EnglishWords.dat</code>:
 *
 * ~~~
 *    int main() {
 *       Lexicon english("EnglishWords.dat");
 *       for (string word : english) {
 *          if (word.length() == 2) {
 *             cout << word << endl;
 *          }
 *       }
 *       return 0;
 *    }
 * ~~~
 */
class Lexicon {

public:

/** \_overload */
    Lexicon();
/**
 * Initializes a new lexicon.  The default constructor creates an empty
 * lexicon.  The second form reads in the contents of the lexicon from
 * the specified data file.  The data file must be in one of two formats:
 * <ol>
 *  <li>A space-efficient precompiled binary format, or
 *  <li>A text file containing one word per line.
 * </ol>
 * The Stanford library distribution
 * includes a binary lexicon file named <code>EnglishWords.dat</code>
 * containing more than 127,000 English words.  The standard code pattern
 * to initialize that lexicon looks like this:
 *
 * ~~~
 *    Lexicon english("EnglishWords.dat");
 * ~~~
 *
 * Sample usages:
 *
 *     Lexicon lex;
 *     Lexicon lex(filename);
 */
   Lexicon(std::string filename);


/**
 * Deallocates any storage associated with this lexicon.
 */
   virtual ~Lexicon();


/**
 * Compares two lexicons for equality.
 * Identical in behavior to the \c == operator.
 *
 * Sample usage:
 *
 *      if (lex1.equals(lex2)) ...
 */
    bool equals(const Lexicon& lex2) const;


/**
 * Returns the number of words contained in this lexicon.
 *
 * Sample usage:
 *
 *     int n = lex.size();
 */
   int size() const;


/**
 * Returns \c true if this lexicon contains no words.
 *
 * Sample usage:
 *
 *     if (lex.isEmpty()) ...
 */
   bool isEmpty() const;


/**
 * Removes all words from the lexicon.
 *
 * Sample usage:
 *
 *     lex.clear();
 */
   void clear();


/**
 * Adds the specified word to the lexicon.
 *
 * Sample usage:
 *
 *     lex.add(word);
 */
   void add(std::string word);


/**
 * Reads a file and adds all of its words to the lexicon.
 * The file must be in one of the two formats specified in the
 * description of the Lexicon class. Moreover, if the file is in
 * the binary format, this lexicon must be empty when this method
 * is called or this method will signal an error.
 *
 * Sample usage:
 *
 *     lex.addWordsFromFile(filename);
 */
   void addWordsFromFile(std::string filename);


/**
 * Returns \c true if \em word is contained in this
 * lexicon.  In the `%Lexicon` class, the case of letters is
 * ignored, so "Zoo" is the same as "ZOO" or "zoo".
 *
 * Sample usage:
 *
 *     if (lex.contains(word)) ...
 */
   bool contains(std::string word) const;


/**
 * Returns \c true if any words in this lexicon begin with the string \em prefix.
 * Like \ref contains, this method ignores the case of letters
 * so that "MO" is a prefix of both "monkey" and "Monday".
 *
 * Sample usage:
 *
 *     if (lex.containsPrefix(prefix)) ...
 */
   bool containsPrefix(std::string prefix) const;


/**
 * Calls the specified function on each word in this lexicon.
 *
 * Sample usage:
 *
 *     lexicon.mapAll(fn);
 */
   void mapAll(void (*fn)(std::string)) const;
   void mapAll(void (*fn)(const std::string &)) const;
   template <typename FunctorType>
   void mapAll(FunctorType fn) const;


    /** \_overload */ // Not really. Just a comment-hack for Doxygen.
    bool operator !=(const Lexicon& lex2) const;
/*
 * Relational operators to compare two lexicons to see if they have the same elements.
 *
 * Sample usages:
 *
 *      if (lex1 == lex2) ...
 *      if (lex1 != lex2) ...
 */
    bool operator ==(const Lexicon& lex2) const;


/*
 * Additional Lexicon operations
 * -----------------------------
 * In addition to the methods listed in this interface, the Lexicon
 * class supports the following operations:
 *
 *   - Deep copying for the copy constructor and assignment operator
 *   - Iteration using the range-based for statement and STL iterators
 *
 * All iteration is guaranteed to proceed in alphabetical order.  All
 * words in the lexicon are stored in lowercase.
 */

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

#ifdef _WIN32
#define LITTLE_ENDIAN 1
#define BYTE_ORDER LITTLE_ENDIAN
#endif

#pragma pack(1)
   struct Edge {
#if defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN
      unsigned long letter:5;
      unsigned long lastEdge:1;
      unsigned long accept:1;
      unsigned long unused:1;
      unsigned long children:24;
#else
      unsigned long children:24;
      unsigned long unused:1;
      unsigned long accept:1;
      unsigned long lastEdge:1;
      unsigned long letter:5;
#endif
   };

   Edge *edges, *start;
   int numEdges, numDawgWords;
   Set<std::string> otherWords;

public:

/*
 * Deep copying support
 * --------------------
 * This copy constructor and operator= are defined to make a
 * deep copy, making it possible to pass/return lexicons by value
 * and assign from one lexicon to another.  The entire contents of
 * the lexicon, including all words, are copied.  Making copies is
 * generally avoided because of the expense and thus, lexicons are
 * typically passed by reference.  When a copy is needed, these
 * operations are supported.
 */

   Lexicon(const Lexicon & src);
   Lexicon & operator=(const Lexicon & src);

/*
 * Iterator support
 * ----------------
 * The classes in the StanfordCPPLib collection implement input
 * iterators so that they work symmetrically with respect to the
 * corresponding STL classes.
 */

   class iterator : public std::iterator<std::input_iterator_tag,std::string> {
   private:
      const Lexicon *lp;
      int index;
      std::string currentDawgPrefix;
      std::string currentSetWord;
      std::string tmpWord;
      Edge *edgePtr;
      Stack<Edge *> stack;
      Set<std::string>::iterator setIterator;
      Set<std::string>::iterator setEnd;

      void advanceToNextWordInDawg();
      void advanceToNextWordInSet();
      void advanceToNextEdge();

   public:
      iterator() {
         this->lp = NULL;
      }

      iterator(const Lexicon *lp, bool endFlag) {
         this->lp = lp;
         if (endFlag) {
            index = lp->size();
         } else {
            index = 0;
            edgePtr = NULL;
            setIterator = lp->otherWords.begin();
            setEnd = lp->otherWords.end();
            currentDawgPrefix = "";
            currentSetWord = "";
            advanceToNextWordInDawg();
            advanceToNextWordInSet();
         }
      }

      iterator(const iterator & it) {
         lp = it.lp;
         index = it.index;
         currentDawgPrefix = it.currentDawgPrefix;
         currentSetWord = it.currentSetWord;
         edgePtr = it.edgePtr;
         stack = it.stack;
         setIterator = it.setIterator;
      }

      iterator & operator++() {
         if (edgePtr == NULL) {
            advanceToNextWordInSet();
         } else {
            if (currentSetWord == "" || currentDawgPrefix < currentSetWord) {
               advanceToNextWordInDawg();
            } else {
               advanceToNextWordInSet();
            }
         }
         index++;
         return *this;
      }

      iterator operator++(int) {
         iterator copy(*this);
         operator++();
         return copy;
      }

      bool operator==(const iterator & rhs) {
         return lp == rhs.lp && index == rhs.index;
      }

      bool operator!=(const iterator & rhs) {
         return !(*this == rhs);
      }

      std::string operator*() {
         if (edgePtr == NULL) return currentSetWord;
         if (currentSetWord == "" || currentDawgPrefix < currentSetWord) {
            return currentDawgPrefix + lp->ordToChar(edgePtr->letter);
         } else {
            return currentSetWord;
         }
      }

      std::string *operator->() {
         if (edgePtr == NULL) return &currentSetWord;
         if (currentSetWord == "" || currentDawgPrefix < currentSetWord) {
            tmpWord = currentDawgPrefix + lp->ordToChar(edgePtr->letter);
            return &tmpWord;
         } else {
            return &currentSetWord;
         }
      }

   };

   iterator begin() const {
      return iterator(this, false);
   }

   iterator end() const {
      return iterator(this, true);
   }

private:

   Edge *findEdgeForChar(Edge *children, char ch) const;
   Edge *traceToLastEdge(const std::string & s) const;
   void readBinaryFile(std::string filename);
   void deepCopy(const Lexicon & src);
   int countDawgWords(Edge *start) const;

   unsigned int charToOrd(char ch) const {
      return ((unsigned int)(tolower(ch) - 'a' + 1));
   }

   char ordToChar(unsigned int ord) const {
      return ((char)(ord - 1 + 'a'));
   }
   friend std::ostream& operator <<(std::ostream& os, const Lexicon& lex);
};

template <typename FunctorType>
void Lexicon::mapAll(FunctorType fn) const {
   foreach (std::string word in *this) {
      fn(word);
   }
}

/*
 * Hashing function for lexicons
 */
int hashCode(const Lexicon& l);

/*
 * Prints the lexicon to the given output stream.
 */
std::ostream& operator <<(std::ostream& os, const Lexicon& lex);

#endif
