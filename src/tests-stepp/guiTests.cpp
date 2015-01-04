/*
 * Test file for verifying the Stanford C++ lib GUI functionality.
 */

#include "testcases.h"
#include "filelib.h"
#include "gbufferedimage.h"
#include "gevents.h"
//#include "gfilechooser.h"
#include "ginteractors.h"
#include "gobjects.h"
//#include "goptionpane.h"
#include "gtypes.h"
#include "gwindow.h"
#include "simpio.h"

#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

void fileDialogTest() {
    // std::cout << "current dir should be \"" << getCurrentDirectory() << "\"" << std::endl;
    std::ifstream input;
    std::string filename = openFileDialog(input, "booyahtitle", "/usr/lib/eclipse");
    std::cout << "you chose \"" << filename << "\"" << std::endl;
}

/*
void goptionpaneTest() {
    GOptionPane::showMessageDialog("Hello, world");
    GOptionPane::showMessageDialog("Here's one with title and type", "booyah", GOptionPane::ERROR);
    GOptionPane::showMessageDialog("special chars [*+*&}{] && || !)@(*&#)(&^%!{ \" \" \" \"}) C:\\Program Files\\Test ;,;:\", ';\"\\//\\// ( ) test 1 2 3 $a $b %a %b %1 %2 http://foo.com/! end");
    
    bool result = GOptionPane::showConfirmDialog("Are you sure?");
    std::cout << "you chose: " << std::boolalpha << result << std::endl;
    result = GOptionPane::showConfirmDialog("Here's one with title and type", "booyah", GOptionPane::OK_CANCEL);
    std::cout << "you chose: " << std::boolalpha << result << std::endl;
    result = GOptionPane::showConfirmDialog("Here's a second one with title and type", "booyah", GOptionPane::YES_NO_CANCEL);
    std::cout << "you chose: " << std::boolalpha << result << std::endl;
    
    std::string input = GOptionPane::showInputDialog("Type something:");
    std::cout << "you typed: \"" << input << "\"" << std::endl;
    input = GOptionPane::showInputDialog("Here's one with a title", "booyah");
    std::cout << "you typed: \"" << input << "\"" << std::endl;
    
    Vector<std::string> choices;
    choices += "a", "bb", "ccc", "dd";
    std::string option = GOptionPane::showOptionDialog("choose!", choices, "booyah", "ccc");
    std::cout << "you chose: \"" << option << "\"" << std::endl;
}
*/


static void border(GBufferedImage* img) {
    int color = 0xdddddd;
    int w = img->getWidth();
    int h = img->getHeight();
    // top/bottom
    for (int x = 0; x < w; x++) {
        img->setRGB(x, 0, color);
        img->setRGB(x, h-1, color);
    }
    // left/right
    for (int y = 0; y < h; y++) {
        img->setRGB(0, y, color);
        img->setRGB(w-1, y, color);
    }
}



void gbufferedImageTest() {
    GWindow gw(500, 500);
    gw.setWindowTitle("Buffered Image Test");

    GButton* button1 = new GButton("Click Me 1");
    gw.add(button1, 250, 80);
    //GButton* button2 = new GButton("Click Me 2");
    //gw.addToRegion(button2, "NORTH");

    GLabel* label = new GLabel("test!");
    gw.add(label, 10, 60);
    
    std::cout << "About to construct GBufferedImage." << std::endl;
    GBufferedImage* img = new GBufferedImage(10, 80, 200, 250);
    std::cout << "Done constructing GBufferedImage (black background)." << std::endl;
    gw.add(img, 50, 50);
    // gw.addToRegion(img, "SOUTH");
    gw.setVisible(true);
    getLine("Enter to continue");

    // fill
    img->fill(0xff00ff);  // purple

    std::cout << "About to setRGB (yellow) on GBufferedImage (inset 5)." << std::endl;
    for (int y = 5; y < img->getHeight() - 5; y++) {
        for (int x = 5; x <= img->getWidth() - 5; x++) {
            img->setRGB(x, y, "yellow");
        }
    }
    std::cout << "Done setting RGB (yellow) on GBufferedImage." << std::endl;
    border(img);
    getLine("Enter to continue");
    
    std::cout << "About to resize (100, 50) on GBufferedImage." << std::endl;
    img->resize(100, 50);
    border(img);
    getLine("Enter to continue");

    std::cout << "About to resize (200, 80) on GBufferedImage." << std::endl;
    img->resize(200, 80);
    border(img);
    getLine("Enter to continue");

    std::cout << "About to setRGB (green) on GBufferedImage (inset 10)" << std::endl;
    for (int y = 10; y < img->getHeight() - 10; y++) {
        for (int x = 10; x <= img->getWidth() - 10; x++) {
            img->setRGB(x, y, "green");
        }
    }
    border(img);
    getLine("Enter to continue");

    std::cout << "About to remove other shit." << std::endl;
    getLine("Enter to continue");
    gw.remove(button1);
    gw.remove(label);
    //gw.removeFromRegion(button2, "NORTH");
    
    std::cout << "About to remove GBufferedImage." << std::endl;
    getLine("Enter to continue");
    gw.remove(img);
    // gw.removeFromRegion(img, "SOUTH");
    getLine("Enter to continue");
    std::cout << "Test complete." << std::endl;
    std::cout << std::endl;
}

