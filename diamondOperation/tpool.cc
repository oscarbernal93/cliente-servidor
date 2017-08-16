#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <thread>
#include <iostream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <math.h>

#define NUM_SIZE 5

using namespace std;
using Vector = vector<int>;

//Sparce Matrix

// codigo 4.5
template<typename T>
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
        while(!work_queue.empty())
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
    void submit(FunctionType f, Vector &arg)
    {
        std::function<void()> my_f = std::bind(f, ref(arg));
        work_queue.push(my_f);
    }
};

//funcion que da formato a los numeros
string format(int n){
  // esta funcion devuelve una cadena con suficientes
  // caracteres c para que al ponerlos a la derecha del
  // numero n su representacion sea de z caracteres
  int z = NUM_SIZE;
  char c = ' ';
  //condicion para negativos y 0
  n = (n<0)? abs(n)*10 : (n==0)? 1 : n;
  //comienzo de la funcion (es recursiva)
  string r = "";
  //condicion de parada
  if (n < pow(10,z)){
    r = c + format(n*10);
    return r;
  }else{
    return r;
  }
}

//funcion que imprime el vector
void print(Vector &v)
{
  //se recorre valor a valor el vector, imprimiendo con formato
  cout << "|";
  for(int n : v) {
      string s = format(n) + to_string(n) + "|";
      cout << s;
    }
  cout << endl;
}

//funcion que imprime una linea del tamaño de impresion del vector
void print_br(Vector &v)
{
  int z = NUM_SIZE;
  cout <<"+";
  for (int n : v){
    for (int i = 0; i <= z; ++i){
      cout << "-";
    }
    cout <<"+";
  }
  cout << endl;
}

void add(Vector &v)
{
  for(int &n : v) {
      n += 2;
    }
    print(v);
}

int main(int argc, char const *argv[])
{
  // se crea el vector
  Vector v = {19,-832,234,-155, -247, 0};
  // se imprime el estado inicial
  print_br(v);
  print(v);
  print_br(v);
  //se comienza el pooling
  {
    thread_pool p;
    for (int i = 0; i < 10; ++i) { p.submit(add,v); }
  }
  print_br(v);
  return 0;
}

