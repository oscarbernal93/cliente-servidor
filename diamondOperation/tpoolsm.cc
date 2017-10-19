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
#define INF numeric_limits<int>::max()

#define FILE "USA-road-d.NY.sort.gr"
#define NODES 264346
#define LIMIT 19

using namespace std;
using namespace std::chrono;
using Vector = vector<int>;

//Sparce Matrix
class SparseMatrix {
private:
  vector<int> val;
  vector<int> colInd;
  vector<int> rowPtr;
  int counter;
public:
  int rows;
  int cols;
  SparseMatrix(int r, int c) : rows(r), cols(c), rowPtr(r + 1, 0), counter(0) {}
  //se tuvo que volver const a get para poder llamarlo desde la multiplicacion
  //ya que la referencia a la matriz b se recibe como const
  int get(int r, int c) const
  {
    // Retorna el elemento que hay en la posicion (r,c)
    //busca donde empieza y donde termina la fila
    int opening = rowPtr[r];
    int ending = rowPtr[r+1];
    int result = INF;
    //recorre las columnas de la fila buscando la solicitada
    for (int i = opening; i < ending; i++) {
      if (colInd[i] == c) {
        //si la encuentra deja de recorrer
        result = val[i];
        break;
      }
    }
    // si recorrio toda la fila y no encontro la columna, result es 0
    // si se escribio algun valor en result, lo retorna
    return result;
  }
  void set(int v, int r, int c)
  {
    //redefinicion del set
    //se asume que la fuente esta ordenada
    val.push_back( v );
    colInd.push_back( c );
    rowPtr[r+1]=counter+1;
    counter++;
    return;
  }
  SparseMatrix mult(const SparseMatrix& b)
  {
    // Multiplica esta matriz con la matriz b
    // se crea la matriz resultado, de filas como la propia matriz t de columnas como la segunda
    int r = rows;
    int c = b.cols;
    SparseMatrix result(r,c);
    for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
        // aca se comienza, para cada posicion i,j de la matriz resultado
        // el calculo de la multiplicacion de matrices.
        int opening = rowPtr[i];
        int ending = rowPtr[i+1];
        int value = 0;
        for (int k = opening; k < ending; k++) {
          //se recorren las columnas con valores
          int tc = colInd[k];
          int v1 = val[k];
          // aca me estaba equivocando en el orden fila/columna
          // que para b se debe tomar columna/fila, ya que se recorre al contrario :P
          int v2 = b.get(tc,j);
          value += v1*v2;
        }
        if (value != 0) {
          result.set(value,i,j);
        }
        //fin del proceso de multiplicacion
      }
    }
    return result;
  }
  void mult_row(int a,Vector& rv,Vector& rc)
  {
    int x1 = rowPtr[a];
    int y1 = rowPtr[a+1];
    //se recorren las columnas correspondientes
    //a las filas que tienen valores
    //
    for (int b = 0; b < rows; ++b)
    {
      //para cada columna
      int x2 = rowPtr[b];
      int y2 = rowPtr[b+1];
      int r=0; //valor
      for (int i = x1; i < y1; ++i)
      {
        //se recorre cada elemento de la columna
        for (int j = x2; colInd[i] >= colInd[j] and j < y2; ++j)
        {
          //busca si existe un valor en la fila correspondiente
          if (colInd[i] == colInd[j])
          {
            //aqui se efectua la multiplicacion
	    r = min(r,val[i] + val[j]);
            //r += val[i]*val[j];
          }
        }
      }
      if (r != 0)
      {
          //si luego de calcular, hay algun valor
        rv.push_back(r);
        rc.push_back(b);
      }
    }
    return;
  }
  void print()
  {
    // imprime la matriz dispersa como los 3 vectores
    //se imprimen los valores
    cout << "Values = ";
    for (const auto i: val)
      cout << i << '\t';
    cout << endl;
    //se imprimen las columnas
    cout << "ColInd = ";
    for (const auto i: colInd)
      cout << i << '\t';
    cout << endl;
    //se imprimen las filas
    cout << "RowPtr = ";
    for (const auto i: rowPtr)
      cout << i << '\t';
    cout << endl;
  }
};

// codigo 4.5
template <typename T>
class threadsafe_queue
{
  private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
  public:
    threadsafe_queue()
    {}
    threadsafe_queue(threadsafe_queue const& other)
    {
      std::lock_guard<std::mutex> lk(other.mut);
      data_queue=other.data_queue;
    }

    void push(T new_value)
    {
      std::lock_guard<std::mutex> lk(mut);
      data_queue.push(new_value);
      data_cond.notify_one();
    }

    void wait_and_pop(T& value)
    {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk,[this]{return !data_queue.empty();});
      value=data_queue.front();
      data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
      std::unique_lock<std::mutex> lk(mut);
      data_cond.wait(lk,[this]{return !data_queue.empty();});
      std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
      data_queue.pop();
      return res;
    }

    bool try_pop(T& value)
    {
      std::lock_guard<std::mutex> lk(mut);
      if(data_queue.empty())
        return false;
      value=data_queue.front();
      data_queue.pop();
      return true;
    }

    std::shared_ptr<T> try_pop()
    {
      std::lock_guard<std::mutex> lk(mut);
      if(data_queue.empty())
        return std::shared_ptr<T>();
      std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
      data_queue.pop();
      return res;
    }

    bool empty() const
    {
      std::lock_guard<std::mutex> lk(mut);
      return data_queue.empty();
    }
};

// codigo de la pagina 248
class join_threads
{
  std::vector<std::thread>& threads;
  public:
    explicit join_threads(std::vector<std::thread>& threads_):
    threads(threads_)
    {}

    ~join_threads()
    {
      for(unsigned long i=0;i<threads.size();++i)
      {
        if(threads[i].joinable())
          threads[i].join();
      }
    }
};

// codigo 9.1
class thread_pool
{
  std::atomic_bool done;
  threadsafe_queue<std::function<void()> > work_queue;
  std::vector<std::thread> threads;
  join_threads joiner;

  void worker_thread()
  {
    while(!done)
    {
      std::function<void()> task;
      if (work_queue.try_pop(task))
      {
        task();
      }
      else
      {
        std::this_thread::yield();
      }
    }
  }
  public:
    thread_pool():
    done(false),joiner(threads)
    {
      unsigned const thread_count=std::thread::hardware_concurrency();
      try
      {
        for(unsigned i=0;i<thread_count;++i)
        {
          threads.push_back(
            std::thread(&thread_pool::worker_thread,this));
        }
      }
      catch(...)
      {
        done=true;
        throw;
      }
    }

    ~thread_pool()
    {
      done=true;
    }

    template<typename FunctionType>
    void submit(FunctionType f, int a, SparseMatrix &m)
    {
      std::function<void()> my_f = std::bind(f, a, ref(m));
      work_queue.push(my_f);
    }
};

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
