/* A one-time hack to convert usage examples in Stanford CPP lib headers
 * to a slightly different Doxygen format after running "convert-comments-to-doxygen"
 * had converted them to Doxygen format. Hindsight!
 *
 * @author Jeff Lutgen
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <cctype>
#include "strlib.h"
#include "filelib.h"
#include "vector.h"
//#include "console.h"

using namespace std;

int main()
{
    //setConsolePrintExceptions(true);
    bool inComment = false;
    bool inUsage = false;
    bool convert = true;
    int lineNumber = 0;

    int argc = getArgumentCount();
    char **argv = getArguments();
    ifstream ifs;
    ofstream ofs;

    if (argc != 2) error("exactly one program argument required");
    ifs.open(argv[1]);
    if (!ifs) error(string("could not open given file") + argv[1]);

    Vector<string> lines;
    Vector<string> newLines;
    Vector<string> usages;
    int pos;
    readEntireFile(ifs, lines);
    for (string line : lines) {
        lineNumber++;
        string trLine = trim(line);
        if (startsWith(trLine, "/*")) {
            inComment = true;
            newLines.add(line);
            if (endsWith(trLine, "*/"))
                inComment = false;
            continue;
        }

        if (inComment) {
            string trLine = trim(line);
            if (startsWith(trLine, "*/")) {
                if (usages.size() > 0) {
                    newLines.add(" *");
                    if (usages.size() == 1)
                        newLines.add(" * Sample usage:");
                    else
                        newLines.add(" * Sample usages:");
                    newLines.add(" * ~~~");
                    for (string usage : usages) {
                        newLines.add(string(" * " + usage));
                    }
                    usages.clear();
                    newLines.add(" * ~~~");
                }
                newLines.add(line);
                inComment = false;
                inUsage = false; // just in case
                convert = false;
                continue;
            }
            // we're in a middle line of a comment
            if (stringIndexOf(line, "File: ")>=0) {
                convert = true;
                int last = newLines.size() - 1;
                if (!startsWith(trim(newLines[last]), "/*")) {
                    cerr << argv[1] << ": " << lineNumber << endl;
                    error("File comment: prev line not /*");
                }
                newLines[last] = "/**";
                stringReplaceInPlace(line, "File: ", "@file ", 1);
                newLines.add(line);
                newLines.add(" *");
                continue;
            }
            if (stringIndexOf(line, "Method: ")>=0
                    || stringIndexOf(line, "Function: ")>=0
                    || stringIndexOf(line, "Constructor: ")>=0
                    || stringIndexOf(line, "Destructor: ")>=0
                    || stringIndexOf(line, "Class: ")>=0
                    || stringIndexOf(line, "Type: ")>=0
                    || stringIndexOf(line, "Operator: ")>=0
                    || stringIndexOf(line, "Friend method: ")>=0) {
                convert = true;
                int last = newLines.size() - 1;
                if (!startsWith(trim(newLines[last]), "/*")) {
                    cerr << argv[1] << ": " << lineNumber << endl;
                    error("prev line not /*");
                }
                newLines[last] = "/**";
                continue;
            }
            if (convert && (pos = stringIndexOf(line, "Usage: "))>=0) {
                inUsage = true;
                usages.add(line.substr(pos+7));
                continue;
            }
            if (convert && stringIndexOf(line, "* -----")>=0 ) {
                if (inUsage)
                    inUsage = false;
                continue;
            }
            if (convert && inUsage) {
                pos = stringIndexOf(line, "*");
                if (pos < 0) error("this shouldn't happen");
                usages.add(trim(line.substr(pos+1)));
                continue;
            }
            if (convert && (startsWith(trLine, "*<pre>") || startsWith(trLine, "*</pre>"))) {
                newLines.add(" * ~~~");
                continue;
            }
            // keep comment line as is
            newLines.add(line);

        } else { // not in comment, so copy line verbatim
            newLines.add(line);
        }
    }

    ifs.close();

    string newFilename = string("/tmp/") + getTail(argv[1]);
    cerr << newFilename << endl;
    ofs.open(newFilename.c_str());
    if (!ofs) error("could not open output file");
    for (string line : newLines)
        ofs << line << endl;
    ofs.close();

    return 0;
}
