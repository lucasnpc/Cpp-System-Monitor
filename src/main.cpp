#include "ncurses_display.h"
#include "system.h"
#include "iostream"
#define printVariableNameAndValue(x) std::cout<<"The name of variable **"<<(#x)<<"** and the value of variable is => "<<x<<"\n"

int main() {
  int abc = 123;
  printVariableNameAndValue(abc);
  System system;
  NCursesDisplay::Display(system);
}