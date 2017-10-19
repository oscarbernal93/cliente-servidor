#ifndef _MYLIB_HH_
  #define _MYLIB_HH_

#include <iostream>
#include <string>

#define ZERO 0

using namespace std;

class MyClass {
  protected:
    int privated;

  public:
    MyClass(string filename);
    int funny(string name);
};

void my_func(string my_str);

#endif
