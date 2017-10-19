#ifndef _DIAMOND_HH_
  #define _DIAMOND_HH_

#include "sparse_matrix.hh"
#include "safe_queue.hh"
#include "thread_pool.hh"
#include <iostream>
#include <string>

#define FILE "USA-road-d.NY.sort.gr"
#define NODES 264346
#define LIMIT 19

using namespace std;
using namespace std::chrono;
using Vector = vector<int>;

void action(int a,SparseMatrix &m);
void load(SparseMatrix &m, string source);

#endif
