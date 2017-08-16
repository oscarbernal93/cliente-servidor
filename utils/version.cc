#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include "zmq.h"

using namespace std;
using namespace zmqpp;

int main (int argc, char const *argv[]) {
  int major, minor, patch;
  zmq_version(&major, &minor, &patch);
  printf("Installed ZeroMQ version: %d.%d.%d\n", major, minor, patch);
  cout << "Installed ZMQPP version: " << version() << endl;
  return 0;
}
