#ifndef _APOLO_HH_
  #define _APOLO_HH_

#include <chrono>
#include <thread>
#include <SFML/Audio.hpp>
#include <glob.h>
#include <iostream>
#include <string>

#define ZERO 0

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

/*
class MyClass {
  protected:
    int privated;

  public:
    FileSplitter(string filename);
    int funny(string name);
};
*/

vector<string> files_in(string dir_name);

#endif
