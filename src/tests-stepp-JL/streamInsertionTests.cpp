/*
 * @file streamInsertionTests.cpp
 *
 * @author Jeff Lutgen
 *
 */

#include "testcases-jl.h"
#include "simpio.h"
#include "lexicon.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// The << operator for Lexicon isn't implemented yet, so, despite its name,
// this function doesn't test that operator.
void streamInsertionLexiconTest() {
    Lexicon lex;
    //    cout << "empty lexicon: " << lex << endl;
    lex.add("alpher");
    //    cout << "1-item lexicon: " << lex << endl;
    lex.add("beter");
    lex.add("gammer");
    //    cout << "3-item lexicon: " << lex << endl;
    //    cout << "looping over lexicon..." << endl;
    for (string s : lex) {
        cout << s << endl;
    }
}

