/*
 * A one-time hack to convert comments in Stanford CPP lib headers
 * to Doxygen format.
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
    bool inUsage = false;
    bool lookForFence = false;
    int fenceCount = 0;

    int argc = getArgumentCount();
    char **argv = getArguments();
    ifstream ifs;
    ofstream ofs;

    if (argc != 2) error("exactly one program argument required");
    ifs.open(argv[1]);
    if (!ifs) error(string("could not open given file") + argv[1]);

    Vector<string> lines;
    Vector<string> newLines;
    readEntireFile(ifs, lines);
    for (string line : lines) {
        string trLine = trim(line);
        if (startsWith(trLine, "* Sample usage")) {
            inUsage = true;
            lookForFence = true;
            newLines.add(line);
            continue;
        }
        if (lookForFence && stringIndexOf(line, "* ~~~") < 0) {
            inUsage = false;
            lookForFence = false;
        }
        if (inUsage) {
            string trLine = trim(line);
            if (startsWith(trLine, "* ~~~")) {
                fenceCount++;
                if (fenceCount == 1) {
                    lookForFence = false;
                    newLines.add(line.substr(0, stringIndexOf(line, "* ~~~") + 1)); // just keep "   *"
                } else if (fenceCount == 2) {
                    // kill this line
                    fenceCount = 0;
                    inUsage = false;
                }
                continue;
            }
            // add 4 spaces before sample code
            if (fenceCount == 1) {
                stringReplaceInPlace(line, "*", "*    ", 1);
                newLines.add(line);
            }
            continue;
        } else { // not in usage section, so copy line verbatim
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
