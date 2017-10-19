#ifndef _SPARSEMATRIX_HH_
  #define _SPARSEMATRIX_HH_

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>

#define ZERO 0
#define INF 2147483647

using namespace std;
using Vector = vector<int>;

class SparseMatrix {
protected:
  vector<int> val;
  vector<int> colInd;
  vector<int> rowPtr;
  int counter;
public:
  int rows;
  int cols;
  SparseMatrix(int r, int c) : rows(r), cols(c), rowPtr(r + 1, 0), counter(0) {}
  int get(int r, int c) const;
  void set(int v, int r, int c);
  SparseMatrix mult(const SparseMatrix& b);
  void mult_row(int a,Vector& rv,Vector& rc);
  void print();
};

#endif
