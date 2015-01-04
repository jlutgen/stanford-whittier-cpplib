#include <iostream>
#include "gwindow.h"
#include "console.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "simpio.h"

using namespace std;

GWindow *gw;

void test_save() {
    GBufferedImage *gbi = new GBufferedImage(10, 10, 100, 100, "red");
    //gw->add(gwi);
    gbi->save("norcos.png");
    gbi->save("norcos.gif");
    gbi->save("norcos.bmp");
    gbi->save("norcos.jpg");
    gbi->save("norcos.tiff");
    gbi->save("norcos.pict");
    gbi->save("norcos.gif89");
}

void test_load() {
    GBufferedImage *gbi = new GBufferedImage();
    gbi->load("images/homer.png");
    //gbi->load("images/avatar.gif");
    //gbi->load("images/earth.jpg");
    //gbi->load("images/test.bmp");
    //gbi->load("EnglishWords.dat");
    gw->add(gbi, 10, 10);
}

void test_diff() {
    //gw->add(new GLabel("original"), 10, 30);
    //gw->add(new GLabel("modified"), 140, 30);
    //gw->add(new GLabel("diff"), 250, 30);
    GBufferedImage *original = new GBufferedImage(100, 100, 0xff0000);
    GBufferedImage *modified = new GBufferedImage(100, 100, 0xff0000);
    GBufferedImage *diff = original->diff(*modified);
    gw->add(original, 10, 50);
    //gw->add(new GLabel("fork1"), 140, 170);
    gw->add(modified, 140, 50);
    //gw->add(new GLabel("fork2"), 150, 170);
    gw->add(diff, 250, 50);


}

int main() {
    //setConsolePrintExceptions(true);
    //cout << "message" << endl;
    getLine("ENTER to continue");
    gw = new GWindow(410, 410);
    //test_save();
    //test_load();
    test_diff();
    //while (true) {}
    return 0;
}
