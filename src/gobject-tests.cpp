#include <iostream>
#include <unistd.h>
#include "gobjects.h"
#include "ginteractors.h"
#include "console.h"
#include "gevents.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "filelib.h"
#include "gmath.h"
#include "gtextarea.h"

using namespace std;

GWindow *gw;
string colors[13] = {"BLACK", "BLUE", "CYAN", "DARK_GRAY", "GRAY", "GREEN", "LIGHT_GRAY", "MAGENTA", "ORANGE", "PINK",
                   "RED", "WHITE", "YELLOW"};

void cycleObjects(Vector<GObject *> &objs, bool forward=true);
void moveTopDown(Vector<GObject *> & objs);
void moveBottomUp(Vector<GObject *> & objs);

void test_front_back() {
    string colors[3] = {"blue", "red", "green"};
    const int objWidth = 100;
    const int objHeight = 100;
    const int x0 = 100;
    const int y0 = 100;
    const int dx = 40;
    const int dy = 0;
    GButton *forward = new GButton("Forward");
    GButton *backward = new GButton("Backward");
    GButton *stepUp = new GButton("Backmost +1");
    GButton *stepDown = new GButton("Frontmost -1");
    Vector<GObject *> objects;
    gw->addToRegion(forward, "south");
    gw->addToRegion(backward, "south");
    gw->addToRegion(stepUp, "south");
    gw->addToRegion(stepDown, "south");
//    cout << "button sizes: " << endl;
//    cout << forward->getSize().toString() << endl;
//    cout << backward->getSize().toString() << endl;
//    cout << stepUp->getSize().toString() << endl;
//    cout << stepDown->getSize().toString() << endl;
    GCompound *compound = new GCompound();
    for (int i=0; i<3; i++) {
        GOval *oval = new GOval(x0+i*dx, y0+i*dy, objWidth, objHeight);
        if (i==1)
            oval->setLocation(oval->getLocation().getX(), oval->getLocation().getY() + 50);
        oval->setFilled(true);
        oval->setFillColor(colors[i]);
        objects.add(oval);
        compound->add(oval);
    }
    gw->add(new GLabel("Click in window to quit", 10, 30));
    gw->add(compound);
    while(true) {
        GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
        if (e.getEventType() == MOUSE_CLICKED)
                break;
        string cmd = ((GActionEvent) e).getActionCommand();
        if (cmd == "Forward")
            cycleObjects(objects);
        if (cmd == "Backward")
            cycleObjects(objects, false);
        if (cmd == "Backmost +1")
            moveBottomUp(objects);
        if (cmd == "Frontmost -1")
            moveTopDown(objects);
    }

}

void cycleObjects(Vector<GObject *> &objs, bool forward) {
    if (objs.isEmpty()) return;
    if (forward) {
        int lastIndex = objs.size() - 1;
        GObject *last = objs[lastIndex];
        for (int i = lastIndex; i>=1; i--) {
            objs[i] = objs[i-1];
        }
        objs[0] = last;
        objs[0]->sendToBack();
    } else {
        int lastIndex = objs.size() - 1;
        GObject *first = objs[0];
        for (int i = 0; i<lastIndex; i++) {
            objs[i] = objs[i+1];
        }
        objs[lastIndex] = first;
        objs[lastIndex]->sendToFront();
    }
}

void moveBottomUp(Vector<GObject *> & objs) {
    if (objs.size() < 2) return;
    objs[0]->sendForward();
    GObject *tmp = objs[1];
    objs[1] = objs[0];
    objs[0] = tmp;
}

void moveTopDown(Vector<GObject *> & objs) {
    int lastIndex = objs.size() - 1;
    if (lastIndex < 1) return;
    objs[lastIndex]->sendBackward();
    GObject *tmp = objs[lastIndex];
    objs[lastIndex] = objs[lastIndex - 1];
    objs[lastIndex - 1] = tmp;
}

void test_image() {
    GImage *im = new GImage("avatar.gif"); // in images subdirectory
    GImage *webIm = new GImage("http://tikiloungetalk.com/wp-content/uploads/2010/11/speedracer-old.jpg");

#ifdef _WIN32
    GImage *absPathIm = new GImage("C:/Qt/Qt5.3.2/Tools/QtCreator/share/qtcreator/qmlicons/Qt/16x16/CountBubble.png");
#else
    GImage *absPathIm = new GImage("/usr/share/doc/cups/images/smiley.jpg");
#endif

    gw->add(im, 10, 10);
    int x = 10 + im->getWidth();
    gw->add(webIm, x, 10);
    x += webIm->getWidth();
    gw->add(absPathIm, x, 10);
}

void test_focus() {
    GWindow *gw2 = new GWindow(200, 200);
    getLine("ENTER to start focus test");
    for (int i=0; i<5; i++) {
        gw->requestFocus();
        pause(1000);
        gw2->requestFocus();
        pause(1000);
    }
    gw2->close();
}

void test_draw() {
    double width = gw->getWidth();
    double height = gw->getHeight();
    const int mod = 200;
    int shift;
    for (int i=0; i<1000; i++) {
        gw->clear();
        shift = i%mod - mod/2;
        gw->drawLine(0 + shift, height / 2, width / 2 + shift, 0);
        gw->drawLine(width / 2 + shift, 0, width + shift, height / 2);
        gw->drawLine(width + shift, height / 2, width / 2 + shift, height);
        gw->drawLine(width / 2 + shift, height, 0 + shift, height / 2);
        gw->setColor("BLUE");
        gw->fillRect(width / 4 + shift, height / 4, width / 2, height / 2);
        gw->setColor("GRAY");
        gw->fillOval(width / 4 + shift, height / 4, width / 2, height / 2);
        pause(20);
    }
}

void rotate_about_center(GObject *gobj, double deg) {
    double x = gobj->getLocation().getX();
    double y = gobj->getLocation().getY();
    double cx = x + gobj->getWidth()/2;
    double cy = y + gobj->getHeight()/2;
    double dx =  (cx-x)*cosDegrees(deg) + (cy-y)*sinDegrees(deg) - (cx-x);
    double dy = -(cx-x)*sinDegrees(deg) + (cy-y)*cosDegrees(deg) - (cy-y);
    gobj->rotate(deg);
    gobj->setLocation(x - dx, y - dy);
}

void test_rotate_scale() {
    gw->add(new GLabel("scale(2,1) then rotate(30)"), 100, 200);
    gw->add(new GLabel("rotate(30) then scale(2,1)"), 400, 200);
    GRect *r1 = new GRect(100, 300, 80, 80);
    r1->setFilled(true);
    gw->add(r1);
    GRect *r1a = new GRect(100, 300, 80, 80);
    r1a->setColor("red");
    r1a->rotate(20);
    r1a->scale(2, 0.5);
//    rotate_about_center(r1a, 20);
    gw->add(r1a);

    GRect *r2 = new GRect(400, 300, 80, 80);
    r2->setFilled(true);
    gw->add(r2);
    GRect *r2a = new GRect(400, 300, 80, 80);
    r2a->setColor("red");
    r2a->scale(2, 1);
    r2a->rotate(30);
    gw->add(r2a);
}

void test_label() {
    GLabel *label1 = new GLabel("<html>Line 1<br/>Line 2</html>");
    label1->setFont("Serif-Bold-18");
    label1->setColor("blue");
    gw->addToRegion(label1, "south");
    pause(2000);
    label1->setLabel("Grissy"); // doesn't work
    label1->setFont("Monospaced-Italic-36");
    label1->setColor("red");

    pause(2000);
    GLabel *label2 = new GLabel("<html>Line 1<br/>Line 2</html>");
    label2->setFont("Monospaced-Plain-36");
    label2->setColor("blue");
    gw->add(label2, 200, 200);
    pause(2000);
    label2->setLabel("Grissy");
    pause(2000);
    label2->rotate(45);


//    for (int i=0; i<1000; i++) {
//        GLabel *lab = new GLabel("Accelerate");
//        gw->add(lab, randomInteger(20, 50), randomInteger(20, 50));
//        gw->remove(lab);
//        delete lab;
//    }

}

void test_add_remove_torture() {
    int width = gw->getCanvasWidth();
    int height = gw->getCanvasHeight();
    int numEachObj = 100;

    GCompound *comp = new GCompound();

    GButton *button = new GButton("button");
    comp->add(button);
    comp->add(new GOval(100, 100), 200, 50);
    gw->add(comp);
    GCompound *comp2 = new GCompound();
    GButton *button2 = new GButton("inner");
    comp2->add(new GRect(100, 100));
    comp2->add(button2, 105, 0);
    pause(2000);
    comp->add(comp2, comp->getWidth() + 10, comp->getHeight() + 10);
    pause(2000);
    gw->remove(comp);
    return;


    Vector<GObject *> objs;
    Vector<GObject *> added;
    for (int i = 0; i<numEachObj; i++) {
        int x = randomInteger(0, width);
        int y = randomInteger(0, height);
        int w = randomInteger(1, width/2);
        int h = randomInteger(1, width/2);
        GRect *r = new GRect(x, y, w, h);
        r->setLineWidth(2);
        r->setColor(colors[randomInteger(0, 12)]);
        r->rotate(randomReal(0,90));
        objs.add(r);
    }
    for (int i=0; i<2000; i++) {
        if (randomChance(0.8)) {
            int index = randomInteger(0, objs.size() - 1);
            gw->add(objs[index]);
            added.add(objs[index]);
        }
        pause(1);
        if (randomChance(0.05) && !added.isEmpty()) {
            int index = randomInteger(0, added.size() - 1);
            gw->remove(added[index]);
            added.remove(index);
        }
        pause(1);
    }
}

void test_checkbox_selected_torture() {
    GCheckBox *cb = new GCheckBox("checky");
    cb->setLocation(gw->getWidth()/2, gw->getHeight()/2);
    gw->add(cb);
    for (int i=0; i<4000; i++) {
        bool state = i%2;
        cb->setSelected(state);
        bool gotState = cb->isSelected();
        if (state != gotState) {
            cout << "oops" << endl;
            break;
        }
    }
}

void test_slider_torture() {
    GSlider *slider = new GSlider(0, 500, 250);
    slider->setLocation(gw->getWidth()/2, gw->getHeight()/2);
    gw->add(slider);
    for (int i=0; i<10000; i++) {
        int value = randomInteger(0, 500);
        slider->setValue(value);
        int gotValue = slider->getValue();
        if (value != gotValue) {
            cout << "oops" << endl;
            break;
        }
    }
}

string randomString(int length) {
    string result = "";
    for (int i=0; i<length; i++) {
        result += 'a' + randomInteger(0, 25);
    }
    return result;
}

void test_textfield_torture() {
    GTextField *textField = new GTextField(60);
    textField->setLocation(gw->getWidth()/2-textField->getSize().getWidth()/2, gw->getHeight()/2);
    gw->add(textField);
    for (int i=0; i<1000; i++) {
        string value = randomString(80);
        string rgbString = colors[randomInteger(0,12)];
        textField->setColor(rgbString);
        string gotColor = textField->getColor();
        if (convertColorToRGB(gotColor) != convertColorToRGB(rgbString)) {
            cout << "oops" << endl;
            break;
        }
        textField->setText(value);
        string gotValue = textField->getText();
        if (value != gotValue) {
            cout << "oops" << endl;
            break;
        }
    }
}

void test_textarea() {
    for (int i = 0; i < 1000; i++) {
        GTextArea *textArea = new GTextArea(10, 10, gw->getWidth()-20, gw->getHeight()-20);
        gw->add(textArea);
        textArea->setFont("Serif-Plain-24");
        //textArea->setBackgroundColor("#9060c0");
        string txt = "Here is\nsome text.";
        textArea->setText(txt);
        //textArea->setEditable(false);
        string gotText = textArea->getText();
        if (txt != gotText) {
            cout << "oops" << endl;
            cout << "    txt:" << txt << endl;
            cout << "gotText:" << gotText << endl;
            break;
        }
        gw->remove(textArea);
        delete textArea;
    }
}

void test_chooser_torture() {
    const int numItems = 100;
    GChooser *chooser = new GChooser();
    Vector<string> items;
    for (int i=0; i<numItems; i++) {
        string item = randomString(8);
        items.add(item);
        chooser->addItem(item);
    }
    //chooser->setSize(200, 20);
    chooser->setLocation(gw->getWidth()/2-chooser->getSize().getWidth()/2, gw->getHeight()/2);
    gw->addToRegion(chooser, "north");
    for (int i=0; i<10000; i++) {
        int index = randomInteger(0, numItems-1);
        chooser->setSelectedItem(items[index]);
        string item = chooser->getSelectedItem();
        if (item != items[index]) {
            cout << "oops" << endl;
            break;
        }
    }
}

void test_region_alignment() {
    string align[3] = { "LEFT", "CENTER", "RIGHT" };
    GButton *b1 = new GButton("Life's");
    GButton *b2 = new GButton("Rich");
    GButton *b3 = new GButton("Pageant");
    GButton *b4 = new GButton("Fables");
    GButton *b5 = new GButton("of the");
    GButton *b6 = new GButton("Reconstruction");
    gw->addToRegion(b1, "NORTH");
    gw->addToRegion(b2, "NORTH");
    gw->addToRegion(b3, "NORTH");
    gw->addToRegion(b4, "SOUTH");
    gw->addToRegion(b5, "SOUTH");
    gw->addToRegion(b6, "SOUTH");
    for (int i=0; i<1000; i++) {
        gw->setRegionAlignment("NORTH", align[randomInteger(0, 2)]);
        pause(1);
        gw->setRegionAlignment("SOUTH", align[randomInteger(0, 2)]);
        pause(1);
    }
}

void test_window_torture() {
    for (int i=0; i<10; i++) {
        GWindow *window = new GWindow(200, 200);
        GOval oval(100, 100);
        window->setColor("red");
        window->drawOval(10, 10, 150, 80);
        //pause(100);
        window->close();
        delete window;
    }
}

void test_file_dialog() {
    string type = getLine("(o)pen or (s)ave? ");
    cout << "currentDirectory is: " << getCurrentDirectory() << endl;
    string startDir = getLine("Starting directory (blank for current dir): ");
    string filter = getLine("file filter(s): ");
    if (!endsWith(startDir, '/') && !endsWith(startDir, '\\'))
        startDir += '/';
    string result;
    if (type == "o") {
        ifstream ifs;
        result = openFileDialog(ifs,"Pick a freakin' file!", startDir + filter);
        if (ifs.is_open())
            ifs.close();
    } else {
        ofstream ofs;
        result = openFileDialog(ofs,"Pick a freakin' file!", startDir + filter);
        if (ofs.is_open())
            ofs.close();
    }
    if (result.empty())
        cout << "User cancelled dialog or file could not be opened for reading" << endl;
    else
        cout << "Dialog result: " <<  result << endl;
}

void test_console() {
    int p = 100;
    clearConsole();
    for (int i=8; i<72; i+=3) {
        string font = string("Monospaced-") + integerToString(i);
        setConsoleFont(font);
        cout << font << endl;
        pause(p);
        clearConsole();
    }
    for (int i=8; i<72; i+=3) {
        string font = string("Serif-") + integerToString(i);
        setConsoleFont(font);
        cout << font << endl;
        pause(p);
        clearConsole();
    }
    for (int i=8; i<72; i+=3) {
        string font = string("SansSerif-") + integerToString(i);
        setConsoleFont(font);
        cout << font << endl;
        pause(p);
        clearConsole();
    }
    setConsoleFont("Monospaced-11");
    cout << "Console test ended" << endl;
}

int main() {
    setConsolePrintExceptions(true);
    setConsoleSize(getScreenWidth()-710-10, 300);
    setConsoleLocation(720, 470);
    cout << "Ready." << endl;
    getLine("ENTER to go");
    gw = new GWindow(710, 610);
    gw->setVisible(false);
    while (true) {
        cout << "f) send front/back" << endl;
        cout << "i) image" << endl;
        cout << "F) focus" << endl;
        cout << "d) draw" << endl;
        cout << "R) rotate/scale" << endl;
        cout << "a) add/remove" << endl;
        cout << "c) checkbox" << endl;
        cout << "s) slider" << endl;
        cout << "t) text field" << endl;
        cout << "T) text area" << endl;
        cout << "l) label" << endl;
        cout << "h) chooser" << endl;
        cout << "r) region alignment" << endl;
        cout << "D) file dialog" << endl;
        cout << "w) window create/destroy" << endl;
        cout << "C) console" << endl;
        string cmd = getLine("Command (Enter to quit)?");
        if (cmd.empty()) {
            break;
        } else if (cmd == "f") {
            gw->setVisible(true);
            gw->clear();
            test_front_back();
        } else if (cmd == "i") {
            gw->setVisible(true);
            gw->clear();
            test_image();
        } else if (cmd =="F") {
            gw->setVisible(true);
            gw->clear();
            test_focus();
        } else if (cmd == "d") {
            gw->setVisible(true);
            gw->clear();
            test_draw();
        } else if (cmd == "R") {
            gw->setVisible(true);
            gw->clear();
            test_rotate_scale();
        } else if (cmd == "a") {
            gw->setVisible(true);
            gw->clear();
            test_add_remove_torture();
        } else if (cmd == "l") {
            gw->setVisible(true);
            gw->clear();
            test_label();
        } else if (cmd == "c") {
            gw->setVisible(true);
            gw->clear();
            test_checkbox_selected_torture();
        } else if (cmd == "s") {
            gw->setVisible(true);
            gw->clear();
            test_slider_torture();
        } else if (cmd == "t") {
            gw->setVisible(true);
            gw->clear();
            test_textfield_torture();
        } else if (cmd == "T") {
            gw->setVisible(true);
            gw->clear();
            test_textarea();
        } else if (cmd == "h") {
            gw->setVisible(true);
            gw->clear();
            test_chooser_torture();
        } else if (cmd == "r") {
            gw->setVisible(true);
            gw->clear();
            test_region_alignment();
        } else if (cmd == "D") {
            gw->setVisible(false);
            test_file_dialog();
        } else if (cmd == "w") {
            gw->setVisible(false);
            test_window_torture();
        } else if (cmd == "C") {
            gw->setVisible(false);
            test_console();
            gw->setVisible(true);
        }
    }

    getLine("ENTER to exit");
    exitGraphics();
    return 0;
}
