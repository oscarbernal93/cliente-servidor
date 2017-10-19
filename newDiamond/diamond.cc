#include "diamond.hh"

int main(int argc, char const *argv[])
{
  high_resolution_clock::time_point t1;
  high_resolution_clock::time_point t2;
  t1= high_resolution_clock::now();
  SparseMatrix m(NODES,NODES);
  load(m, FILE);
  t2 = high_resolution_clock::now();
  cout << "matrix reading: " << duration_cast<microseconds>(t2 - t1).count() << " microseconds" << endl;
  //se comienza el ciclo
 
  for (int k=1; k <= LIMIT; k++)
  {
    thread_pool p;
    for (int i = 0; i < NODES; ++i)
    {
      p.submit(action,i,ref(m));
    }
  }
  t1 = high_resolution_clock::now();
  cout << "Diamond Operation " << LIMIT << " times: " << duration_cast<microseconds>(t1 - t2).count() << " microseconds" << endl;
  return 0;
}

void action(int a,SparseMatrix &m)
{
  Vector rv;
  Vector rc;
  m.mult_row(a,rv,rc);
}

void load(SparseMatrix &m, string source)
{

  ifstream fin;
  string line;

  fin.open(source, ios::in);
  while (std::getline(fin, line)) {
    stringstream stream(line);

    char data;
    stream >> data;

    if (data == 'a') {
      size_t r, c, v;
      stream >> r >> c >> v;
      m.set( v , r-1 , c-1 );
    }
  }
}
