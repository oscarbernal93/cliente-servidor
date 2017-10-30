#ifndef _DIAMOND_HH_
  #define _DIAMOND_HH_

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <utility>
#include <unistd.h>
#include <math.h>
#include <fstream>
#include <sstream>
#define INF (numeric_limits<int>::max()/2)-1

#define FILE "USA-road-d.NY.sort.gr"
//define FILE "mintest.sort.gr"
#define NODES 264346
//define NODES 5
#define LIMIT 19

using namespace std;
using namespace std::chrono;
using Vector = vector<int>;

#endif
