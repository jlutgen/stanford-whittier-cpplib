#include "testcases-jl.h"
#include "simpio.h"
#include "lexicon.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void streamInsertionLexiconTest() {
    Lexicon lex;
//    cout << "empty lexicon: " << lex << endl;
    lex.add("alpher");
//    cout << "1-item lexicon: " << lex << endl;
    lex.add("beter");
    lex.add("gammer");
//    cout << "3-item lexicon: " << lex << endl;
    cout << "looping over lexicon..." << endl;
    for (string s : lex) {
        cout << s << endl;
    }
}

