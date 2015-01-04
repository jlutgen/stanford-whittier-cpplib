/**
 * @file tokenscanner.h
 *
 * @brief
 * This file exports a TokenScanner class that divides
 * a string into individual logical units called <b><i>tokens</i></b>.
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

#ifndef _tokenscanner_h
#define _tokenscanner_h

#include <iostream>
#include <string>
#include "private/tokenpatch.h"

/**
 * \enum TokenType
 *
 * This enumerated type defines the values returned by the
 * \ref getTokenType method of the TokenScanner class.
 */

enum TokenType {
    SEPARATOR, ///<.
    WORD,      ///<.
    NUMBER,    ///<.
    STRING,    ///<.
    OPERATOR   ///<.
};


/**
 * This class divides a string into individual tokens.  The typical
 * use of the `%TokenScanner` class is illustrated by the
 * following pattern, which reads the tokens in the string variable
 * \c input:
 *
 * ~~~
 *    TokenScanner scanner(input);
 *    while (scanner.hasMoreTokens()) {
 *       string token = scanner.nextToken();
 *       ... process the token ...
 *    }
 * ~~~
 *
 * The `%TokenScanner` class exports several additional methods
 * that give clients more control over its behavior.  Those methods are
 * described individually in the documentation.
 */
class TokenScanner {

public:

/** \_overload */
   TokenScanner();
/** \_overload */
   TokenScanner(std::string str);
/**
 * Creates a scanner object.  The initial token stream comes from
 * the specified string or input stream, if supplied.  The default
 * constructor creates a scanner with an empty token stream.
 *
 * Sample usages:
 *
 *     TokenScanner scanner;
 *     TokenScanner scanner(str);
 *     TokenScanner scanner(infile);
 */
   TokenScanner(std::istream & infile);


/**
 * Deallocates any storage associated with this scanner.
 */
   virtual ~TokenScanner();


/** \_overload */
   void setInput(std::string str);
/**
 * Sets the token stream for this scanner to the specified string or
 * input stream.  Any previous token stream is discarded.
 *
 * Sample usages:
 *
 *     scanner.setInput(str);
 *     scanner.setInput(infile);
 */
   void setInput(std::istream & infile);


/**
 * Returns \c true if there are additional tokens for this
 * scanner to read.
 *
 * Sample usage:
 *
 *     if (scanner.hasMoreTokens()) ...
 */
   bool hasMoreTokens();


/**
 * Returns the next token from this scanner.  If this method
 * is called when no tokens are available, it returns the empty string.
 *
 * Sample usage:
 *
 *     token = scanner.nextToken();
 */
   std::string nextToken();


/**
 * Pushes the specified token back into this scanner's input stream.
 * On the next call to \ref nextToken, the scanner will return
 * the saved token without reading any additional characters from the
 * token stream.
 *
 * Sample usage:
 *
 *     scanner.saveToken(token);
 */
   void saveToken(std::string token);


/**
 * Returns the current position of this scanner in the input stream.
 * If \ref saveToken has been called, this position corresponds
 * to the beginning of the saved token.  If \c saveToken is
 * called more than once, this method returns -1.
 *
 * Sample usage:
 *
 *     int pos = scanner.getPosition();
 */
   int getPosition() const;


/**
 * Tells the scanner to ignore whitespace characters.  By default,
 * the \ref nextToken method treats whitespace characters
 * (typically spaces and tabs) just like any other punctuation mark
 * and returns them as single-character tokens.
 * Calling
 *
 *     scanner.ignoreWhitespace();
 *
 * changes this behavior so that the scanner ignores whitespace characters.
 *
 * Sample usage:
 *
 *     scanner.ignoreWhitespace();
 */
   void ignoreWhitespace();


/**
 * Tells this scanner to ignore comments.  The scanner package recognizes
 * both the slash-star and slash-slash comment format from the C-based
 * family of languages.  Calling
 *
 *     scanner.ignoreComments();
 *
 * sets the scanner to ignore such comments.
 *
 * Sample usage:
 *
 *     scanner.ignoreComments();
 */
   void ignoreComments();


/**
 * Controls how this scanner treats tokens that begin with a digit.  By
 * default, the \ref nextToken method treats numbers and letters
 * identically and therefore does not provide any special processing for
 * numbers.  Calling
 *
 *      scanner.scanNumbers();
 *
 * changes this behavior so that \c nextToken returns the
 * longest substring that can be interpreted as a real number.
 *
 * Sample usage:
 *
 *     scanner.scanNumbers();
 */
   void scanNumbers();


/**
 * Controls how the scanner treats tokens enclosed in quotation marks.  By
 * default, quotation marks (either single or double) are treated just like
 * any other punctuation character.  Calling
 *
 *     scanner.scanStrings();
 *
 * changes this assumption so that \ref nextToken returns a single
 * token consisting of all characters through the matching quotation mark.
 * The quotation marks are returned as part of the scanned token so that
 * clients can differentiate strings from other token types.
 *
 * Sample usage:
 *
 *     scanner.scanStrings();
 */
   void scanStrings();


/**
 * Adds the characters in \em str to the set of characters
 * legal in a \c WORD token.  For example, calling
 *
 *      addWordCharacters("_")
 *
 * adds the underscore to the
 * set of characters that are accepted as part of a word.
 *
 * Sample usage:
 *
 *     scanner.addWordCharacters(str);
 */
   void addWordCharacters(std::string str);


/**
 * Returns \c true if the character \em ch is valid in a word.
 *
 * Sample usage:
 *
 *     if (scanner.isWordCharacter(ch)) ...
 */
   bool isWordCharacter(char ch) const;


/**
 * Defines a new multicharacter operator.  Whenever
 * \ref nextToken is called when the input stream contains operator
 * characters, the scanner returns the longest possible operator
 * string that can be read at that point.
 *
 * Sample usage:
 *
 *     scanner.addOperator(op);
 */
   void addOperator(std::string op);


/**
 * Reads the next token and makes sure it matches the string
 * \em expected.  If it does not, this function signals
 * an error.
 *
 * Sample usage:
 *
 *     scanner.verifyToken(expected);
 */
   void verifyToken(std::string expected);


/**
 * Returns the type of \em token.  This type will match one of the
 * following constants: <code>EOF</code>,
 * <code>SEPARATOR</code>, <code>WORD</code>, <code>NUMBER</code>,
 * <code>STRING</code>, or <code>OPERATOR</code>.
 *
 * Sample usage:
 *
 *     TokenType type = scanner.getTokenType(token);
 */
TokenType getTokenType(std::string token) const;


/**
 * Reads the next character from this scanner's input stream.
 *
 * Sample usage:
 *
 *     int ch = scanner.getChar();
 */
int getChar();


/**
 * Pushes the specified character back into this scanner's stream.
 * The character must match the last one that was read.
 *
 * Sample usage:
 *
 *     scanner.ungetChar(ch);
 */
void ungetChar(int ch);


/**
 * Returns the string value of a token.  This value is formed by removing
 * any surrounding quotation marks and replacing escape sequences by the
 * appropriate characters.
 *
 * Sample usage:
 *
 *     string str = scanner.getStringValue(token);
 */
   std::string getStringValue(std::string token) const;


/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

private:

/*
 * Private type: StringCell
 * ------------------------
 * This type is used to construct linked lists of cells, which are used
 * to represent both the stack of saved tokens and the set of defined
 * operators.  These types cannot use the Stack and Lexicon classes
 * directly because tokenscanner.h is an extremely low-level interface,
 * and doing so would create circular dependencies in the .h files.
 */

   struct StringCell {
      std::string str;
      StringCell *link;
   };

   enum NumberScannerState {
      INITIAL_STATE,
      BEFORE_DECIMAL_POINT,
      AFTER_DECIMAL_POINT,
      STARTING_EXPONENT,
      FOUND_EXPONENT_SIGN,
      SCANNING_EXPONENT,
      FINAL_STATE
   };

   std::string buffer;              /* The original argument string */
   std::istream *isp;               /* The input stream for tokens  */
   bool stringInputFlag;            /* Flag indicating string input */
   bool ignoreWhitespaceFlag;       /* Scanner ignores whitespace   */
   bool ignoreCommentsFlag;         /* Scanner ignores comments     */
   bool scanNumbersFlag;            /* Scanner parses numbers       */
   bool scanStringsFlag;            /* Scanner parses strings       */
   std::string wordChars;           /* Additional word characters   */
   StringCell *savedTokens;         /* Stack of saved tokens        */
   StringCell *operators;           /* List of multichar operators  */

/* Private method prototypes */

   void initScanner();
   void skipSpaces();
   std::string scanWord();
   std::string scanNumber();
   std::string scanString();
   bool isOperator(std::string op);
   bool isOperatorPrefix(std::string op);

};

#endif
