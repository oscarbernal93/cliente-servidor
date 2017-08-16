#include <iostream>
#include <thread>
#include <vector>
#include <utility>

using namespace std;

using Matrix = vector<vector<int>>;
using Vector = vector<int>;

void mult (const Matrix &m, int col, Matrix &r) {
  //matrix multiplication
  for (int i = 0; i < m.size(); i++)
  {
    int sum = numeric_limits<int>::max();
    for (int j = 0; j < m[0].size(); j++)
    {
      sum = min(sum, m[i][j] + m[j][col]);
    }
    r[i][col] = sum;
  }
}

void print (const Matrix &m) {
  //matrix print
  cout<< "==" << endl;
  for (int i = 0; i < m.size(); ++i)
  {
    for (int j = 0; j < m[i].size(); ++j)
    {
      cout << " " << m[i][j];
    }
    cout << endl;
  }
}

int main() {
  Matrix m = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
            };
  Matrix r(3,Vector(3,0));  // matriz de ceros

  //se lanzan los hilos
  thread t1 = thread(mult,cref(m),0,ref(r));
  thread t2 = thread(mult,cref(m),1,ref(r));
  thread t3 = thread(mult,cref(m),2,ref(r));
  print(r);
  t1.join();
  t2.join();
  t3.join();
  print(r);
  return 0;
}