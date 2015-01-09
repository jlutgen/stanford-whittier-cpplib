

//#include <fstream>
#include <iomanip>
#include <iostream>
#include <unistd.h>
//#include <cctype>
//#include <sstream>
//#include <string>
#include "console.h"
//#include "filelib.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
//#include "graph.h"
//#include "grid.h"
#include "gwindow.h"
#include "map.h"
//#include "random.h"
#include "simpio.h"
#include "strlib.h"
//#include "assign-1-main.h"
//#include "error.h"
//#include "vector.h"
//#include "error.h"
//#include "sound.h"
//#include "gtimer.h"
//#include "platform.h"
//#include "hashset.h"
//#include "point.h"
//#include "sound.h"
//#include "tokenscanner.h"
#include "gbufferedimage.h"
//#include "lexicon.h"
#include "thread.h"
#include <cstdio>

using namespace std;

int main()
{
    int consoleX = 10;
    int consoleY = 10;
  setConsoleSize(600, 400);
  setConsoleFont("Monospaced-14");
//  setConsoleEcho(true);
  setConsoleExitProgramOnClose(false);
//  setConsoleFont("Monospaced-11");
//    setConsoleFont("Consolas-14");
  setConsolePrintExceptions(true);

  GBufferedImage buffImage(500, 500);
  //buffImage.load("checker.png");
  cout << "image size: " << buffImage.getWidth() << ", " << buffImage.getHeight() << endl;


//  extern int _mainFlags;
//  if (_mainFlags & CONSOLE_FLAG) cout << "CONSOLE_FLAG set" << endl;
//  if (_mainFlags & GRAPHICS_FLAG) cout << "GRAPHICS_FLAG set" << endl;
//  if (_mainFlags & JBEMISC_FLAG) cout << "JBEMISC_FLAG set" << endl;


    GWindow gw(800, 300);

    setConsoleLocation(0, 300 + 50);
    gw.setColor("red");
    double winWidth = gw.getWidth();
    double winHeight = gw.getHeight();
    for (int i=0; i<winWidth; i+=100)
      gw.drawLine(i, 0, i, winHeight);
    for (int j=0; j<winHeight; j+=100)
      gw.drawLine(0, j, winWidth, j);
    gw.setColor("black");
    GPolygon *poly = new GPolygon();
    poly->setLineWidth(2);
    poly->setColor("green");
    poly->setLocation(400, 400);
    gw.add(poly);
    poly->addEdge(200, 200);
    //poly->addVertex(300, 300);

//  ifstream ifs;
//  cout << "calling openFileDialog..." << endl;
//  cout << "currentDirectory: " << getCurrentDirectory() << endl;
//  Platform *p = getPlatform();
//  cout << "p->getCurrentDirectory(): " << p->getCurrentDirectory() << endl;
//  setCurrentDirectory("c:/windows");
//  cout << "p->getCurrentDirectory(): " << p->getCurrentDirectory() << endl;
//  cout << openFileDialog(ifs,"title", "\\\\*.txt;*.wav") << endl;
//  return 0;




//  GWindow gw(600,500);
//  GButton *button = new GButton("RED");
//  gw.addToRegion(button, "SOUTH");
//  while (true)
//    {
//      GEvent e = waitForEvent(ACTION_EVENT | CLICK_EVENT);
//      if (e.getEventType() == MOUSE_CLICKED) break;
//      cout << "Please do not press this button again." << endl;
//    }



//pause(200);
  cout << "Screen size: " << getScreenWidth() << "x" << getScreenHeight() << endl;
  cout << "GWindow size: " << gw.getWidth() << "x" << gw.getHeight() << endl;
  cout << "Canvas size before adding interactors: " << gw.getCanvasWidth()
       << "x" << gw.getCanvasHeight() << endl;
  GWindow gw2;

  gw.setWindowTitle("Balthazar");
  gw2.setWindowTitle("Cuthbert");
  GButton *button = new GButton("Shift Rectangle");
  GButton *quitButton = new GButton("Quit");
  GCheckBox *cb = new GCheckBox("check here");
  GChooser *chooser = new GChooser();
  GSlider *slider = new GSlider(0, 100, 30);
  GTextField *textField = new GTextField();
  chooser->addItem("Up");
  chooser->addItem("Down");
  chooser->setSelectedItem("Up");
  chooser->setActionCommand("choosey");
  cb->setActionCommand("gurgle");
  slider->setActionCommand("slidey");
  textField->setActionCommand("texty");
  gw.setColor("red");

  GRect rect = GRect(40, 40, 60, 30);
  rect.setColor("#aa00cc98");
  cout << "rect.getColor(): " << rect.getColor() << endl;
  cout << "2309667788: " << convertRGBToColor(2309667788) << endl;
  cout << "magenta: " << hex << (unsigned) convertColorToRGB("gray") << endl;
  cout << "convertColorToRGB(#66ffc800): " << convertColorToRGB("#66ffc800") << endl;
  cout << "convertColorToRGB(#ffc800): " << convertColorToRGB("#ffc800") << endl;
  rect.setLineWidth(10);
  gw.add(&rect); // foreground layer
  gw.drawRect(89, 90, 29, 29); // background, automatically redrawn

  //G3DRect rect3d(55, 70, true);
  //rect3d.setLineWidth(10);
  //rect3d.setFillColor("red");
  //rect3d.setFilled(false);
  //gw.draw(&rect3d, 100, 100);
  //gw.add(textField, 180, 240);
  gw.addToRegion(textField, "NORTH");
  gw.addToRegion(button, "SOUTH");
  gw.addToRegion(cb, "EAST");
  gw.addToRegion(chooser, "SOUTH");
  gw.addToRegion(quitButton, "south");
  gw.addToRegion(slider, "WEST");
  cout << "Canvas size after adding interactors: " << gw.getCanvasWidth()
       << "x" << gw.getCanvasHeight() << endl;

  for (int i=0; i<winWidth; i+=100)
    gw.drawLine(i, 0, i, winHeight);
  for (int j=0; j<winHeight; j+=100)
    gw.drawLine(0, j, winWidth, j);
  double cx = gw.getCanvasWidth() / 2;
  double cy = gw.getCanvasHeight() / 2;
  double r = 25;
  GArc *pacman = new GArc(cx - r, cy - r, 2 * r, 2 * r, 45, 270);
  pacman->setFilled(true);
  pacman->setFillColor(0x20cccccc);
  gw.add(pacman);


//  gw.add(&buffImage, 0, 0);
  string ans = getLine("remove checkbox? (y/n) ");
  if (startsWith(ans, "y"))
      gw.removeFromRegion(cb, "EAST");

  ans = getLine("Clear console? (y/n) ");
  if (startsWith(ans, "y"))
      clearConsole();

//  pause(1000);
//  buffImage.fill("blue");
//  cout << "buffimage blue is: " << buffImage.getRGBString(0, 0) << endl;

  GButton *naked = new GButton("naked");
  gw.add(naked, 200, 110);

  GEvent e = getNextEvent();
  if (!e.isValid())
      cout << "getNextEvent returned invalid event: " << e.toString() << endl;
  else
      cout << "getNextEvent returned valid event: " << e.toString() << endl;

//    cout << "This program generates timer events." << endl;
//    GTimer timer(2000);
//    timer.start();
//    int ticks = 0;
//    while (true)
//      {
//        GEvent e = waitForEvent(CLICK_EVENT | TIMER_EVENT);
//        if (e.getEventType() == MOUSE_CLICKED) break;
//        cout << "Timer ticked: " << ticks++ << endl;
//      }

//    getLine("\n*** Execution finished. Press [Enter] to close the window(s)... ");
//    closeConsoleAndExit();
//    return 0;

  cout.setf(ios::fixed);
  cout << dec;
  while (true)
    {
      GEvent e = waitForEvent();
      EventType type = e.getEventType();
      if (type==KEY_RELEASED||type==MOUSE_MOVED
          ||type==MOUSE_PRESSED||type==MOUSE_RELEASED)
        continue; // ignore these
      int eclass = e.getEventClass();
      int mods = e.getModifiers();
      string modstr = "";
      if (mods & SHIFT_DOWN) modstr += "SHIFT ";
      if (mods & CTRL_DOWN) modstr += "CTRL ";
      if (mods & META_DOWN) modstr += "META ";
      if (mods & ALT_DOWN) modstr += "ALT ";
      if (mods & ALT_GRAPH_DOWN) modstr += "ALTGRAPH ";
      if (mods & BUTTON1_DOWN) modstr += "BUT1 ";
      if (mods & BUTTON2_DOWN) modstr += "BUT2 ";
      if (mods & BUTTON3_DOWN) modstr += "BUT3 ";

      cout << e.toString() << endl;
      cout << "\tTime: " << setprecision(0) << e.getEventTime() << endl;
      cout << "\tModifiers: " << modstr << endl;

      if (eclass==ACTION_EVENT)
        {
          GActionEvent ae(e);
          string cmd = ae.getActionCommand();
          if (ae.getSource() == quitButton)
              exitGraphics();
          cout << "\tSource: " << ae.getSource()->toString() << " @ " << ae.getSource() << endl;
          cout << "\tActionCommand: " << cmd << endl;
          if (cmd == "choosey")
            {
              cout << "\tItem Selected: " << chooser->getSelectedItem() << endl;
            }
          if (cmd == "slidey")
            {
              cout << "\tCurrent Value: " << slider->getValue() << endl;
            }
          if (cmd == "texty")
            {
              cout << "\tText: " << textField->getText() << endl;
            }
          if (cmd == "gurgle")
            {
              cout << "\tSelected: " << boolToString(cb->isSelected()) << endl;
            }
          if (ae.getSource() == button)
            {
              GPoint p = rect.getLocation();
              if (chooser->getSelectedItem()=="Up")
                {
                  rect.setLocation(p.getX(), p.getY()-5);
                  consoleX -= 10;
                  consoleY -= 10;
                  setConsoleLocation(consoleX, consoleY);
                  cout << "up" << endl;
                }
              else
                {
                  rect.setLocation(p.getX(), p.getY()+5);
                  consoleX += 10;
                  consoleY += 10;
                  setConsoleLocation(consoleX, consoleY);
                  cout << "down" << endl;
                }
            }
        }
      if (eclass==WINDOW_EVENT)
        {
          GWindowEvent we(e);
          cout << "\tTitle: " << we.getGWindow().getWindowTitle() << endl;
          if (we.getEventType() == WINDOW_CLOSED) {

              //break;
          }
        }
      if (eclass==MOUSE_EVENT)
        {
          GMouseEvent me(e);
          cout << "\tWindow Title: " << me.getGWindow().getWindowTitle() << endl;
          cout << "\t(x, y): " << "(" << me.getX() << ", " << me.getY() << ")" << endl;
        }
      if (eclass==KEY_EVENT)
        {
          GKeyEvent ke(e);
          cout << "\tWindow Title: " << ke.getGWindow().getWindowTitle() << endl;
          char keyChar = ke.getKeyChar();
          int keyCode = ke.getKeyCode();
          ostringstream oss;
          oss << "'";
          if (isprint(keyChar)) oss << keyChar;
          else oss << '\\' << oct << int(keyChar);
          oss << "'";
          cout << "\tKeyChar: " << oss.str() << endl;

          oss.str("");
          switch(keyCode)
            {
            case BACKSPACE_KEY:
              oss << "BACKSPACE_KEY";
              break;
            case TAB_KEY:
              oss << "TAB_KEY";
              break;
            case ENTER_KEY:
              oss << "ENTER_KEY";
              break;
            case CLEAR_KEY:
              oss << "CLEAR_KEY";
              break;
            case ESCAPE_KEY:
              oss << "ESCAPE_KEY";
              break;
            case PAGE_UP_KEY:
              oss << "PAGE_UP_KEY";
              break;
            case PAGE_DOWN_KEY:
              oss << "PAGE_DOWN_KEY";
              break;
            case END_KEY:
              oss << "END_KEY";
              break;
            case HOME_KEY:
              oss << "HOME_KEY";
              break;
            case LEFT_ARROW_KEY:
              oss << "LEFT_ARROW_KEY";
              break;
            case UP_ARROW_KEY:
              oss << "UP_ARROW_KEY";
              break;
            case RIGHT_ARROW_KEY:
              oss << "RIGHT_ARROW_KEY";
              break;
            case DOWN_ARROW_KEY:
              oss << "DOWN_ARROW_KEY";
              break;
            case F1_KEY:
              oss << "F1_KEY";
              break;
            case F2_KEY:
              oss << "F2_KEY";
              break;
            case F3_KEY:
              oss << "F3_KEY";
              break;
            case F4_KEY:
              oss << "F4_KEY";
              break;
            case F5_KEY:
              oss << "F5_KEY";
              break;
            case F6_KEY:
              oss << "F6_KEY";
              break;
            case F7_KEY:
              oss << "F7_KEY";
              break;
            case F8_KEY:
              oss << "F8_KEY";
              break;
            case F9_KEY:
              oss << "F9_KEY";
              break;
            case F10_KEY:
              oss << "F10_KEY";
              break;
            case F11_KEY:
              oss << "F11_KEY";
              break;
            case F12_KEY:
              oss << "F12_KEY";
              break;
            case DELETE_KEY:
              oss << "DELETE_KEY";
              break;
            case HELP_KEY:
              oss << "HELP_KEY";
              break;
            default:
              oss << "'";
              if (isprint(keyCode)) oss << char(keyCode);
              else oss << '\\' << oct << keyCode;
              oss << "'";
            }
          cout << "\tKeyCode: " << oss.str() << endl;
        }
    } // while

  return 0;

} // main








