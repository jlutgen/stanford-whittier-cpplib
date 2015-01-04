#include <iostream>
#include "console.h"
//#include "gwindow.h"
#include "error.h"
#include "vector.h"

using namespace std;

int main() {

  //setConsoleExitProgramOnClose(true);
  setConsolePrintExceptions(false);

  //GWindow *gw = new GWindow();

  cout << "Testing old Stanford" << endl;
  try {
    throw 20;
  } catch (string s) {
    // empty
  }

  int n;
  cin >> n;

  Vector<int> v;
  v[100]=99;

  cin >> n;
  cout << n << endl;


  return 0;
}
