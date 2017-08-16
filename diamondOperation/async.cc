#include <iostream>
#include <thread>
#include <future>

using namespace std;

int count(int val) {
  int c=0;
  for (int i = 0; i < val; ++i)
  {
    c = c+1;
  }
  return c;
}

int main(int argc, char const *argv[])
{
  future<int> t = async(launch::async,count,50);
  future<int> s = async(launch::async,count,150);
  cout << t.get() + s.get()<< endl;
  return 0;
}
