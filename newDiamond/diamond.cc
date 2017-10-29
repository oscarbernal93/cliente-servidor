#include "diamond.hh"

//Linked List
typedef struct node {
  int colInd;
  int val;
  struct node * prev;
  struct node * next;
} node;

//Sparce Matrix
class SparseMatrix {
public:
  vector<int> rowPtr;
  vector<node *> pointers;
  int counter;
  int rows;
  int cols;
  SparseMatrix(int r, int c) : rows(r), cols(c), rowPtr(r + 1, 0), pointers (r,NULL), counter(0){}
  //se tuvo que volver const a get para poder llamarlo desde la multiplicacion
  //ya que la referencia a la matriz b se recibe como const
  int get(int r, int c) const
  {
    if (c == r){
      return 0;
    }
    // Retorna el elemento que hay en la posicion (r,c)
    //busca donde empieza y donde termina la fila
    int opening = rowPtr[r];
    int ending = rowPtr[r+1];
    int result = INF;
    //recorre las columnas de la fila buscando la solicitada
    int i = opening;
    node *current = pointers[r];
    while(i < ending) {
      if (current->colInd == c) {
        //si la encuentra deja de recorrer
        result = current->val;
        break;
      }
      //incremento y paso al siguiente
      i++;
      current = current->next;
    }

    // si recorrio toda la fila y no encontro la columna, result es INF
    // si se escribio algun valor en result, lo retorna
    return result;
  }
  //set solo funciona en orden y es rapida
  //se usa al leer la primera matriz del archivo
  void set(int v, int r, int c)
  {
    //redefinicion del set
    //se asume que la fuente esta ordenada
    node *nuevo = (node *) malloc(sizeof(node));
    nuevo->val = v;
    nuevo->colInd = c;
    rowPtr[r+1]=counter+1;
    pointers[r]= nuevo; 
    counter++;
    return;
  }
  //put crea e inserta un nuevo elemento
  void put (int v, int r, int c)
  {
    //intenta encontrar el elemento
    int opening = rowPtr[r];
    int ending = rowPtr[r+1];
    //recorre las columnas de la fila buscando la solicitada
    int i = opening;
    node *current = pointers[r];
    while(i < ending) {
      if (current->colInd == c) {
        //si la encuentra escribe y deja de recorrer
        current->val = v;
        break;
      }
      if (current->colInd > c) {
        //si lo columna en la que esta es mayor a la que quiere escribir
        //es porque se paso  y es necesario insertar un nodo antes
 	node *nuevo = (node *) malloc(sizeof(node));
        node *ans = current->prev;
        ans->next = nuevo;
        nuevo->next = current;
        nuevo->prev = ans;
        current->prev = nuevo;
	//si ademas esta en el primero actualiza el puntero
	if(i == opening)
	{
	  pointers[r] = nuevo;
	}
        // ahora actualiza todas los demas rowPtr
	int j;
	for(j = r+1;j<= rows;j++)
	{
	  rowPtr[j] += 1; 
	}
        break;
      }
      //incremento y paso al siguiente
      i++;
      current = current->next;
    }

  }
  void mult(SparseMatrix &result)
  {
    int x;
    // Multiplica esta matriz con si misma escribiendo el resultado en result
    //cicla sobre el numero de filas llamando a la multiplicacion pequenna
    for(x = 0; x < rows; x++)
    {
      this->row_mult(x,result);
    }
  }
  void row_mult (int row_n, SparseMatrix &result)
  {
    int x,y,cur_col,val_origen,val_destino;
    //para la fila x de result
    x = row_n;
    //y cada columna y de result
    // se calculara el valor x,y
    for (y = 0; y < cols; y++)
    {
      //solo hace esto si x es diferente de y
      // si son iguales es valor es 0 y no se escribe
      if( x != y)
      {
        //valor que se va a calcular en esta ronda
        int value = result.get(x,y);
        //se recorre la fila x en origen
        int opening = rowPtr[x];
        int ending = rowPtr[x+1];
        int i = opening;
        node *current = pointers[x];
        while(i < ending)
        {
          cur_col = current->colInd;
          val_origen = current->val;
          val_destino = this->get(cur_col,y);
          //se calcula el nuevo value
          value = min( value, val_origen + val_destino );
          //incremento y paso al siguiente
          i++;
          current = current->next;
        }
        //una vez termina de recorrer la fila origen escribe el valor
        result.put(value,x,y);
      }  
    }
  }

  void print()
  {
    int x,y;
    for(x = 0; x < rows; x++)
    {
      for(y = 0; y < cols; y++)
      {
        cout << "\t" << this->get(x,y);
      }
      cout << endl;
    }
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
  join_threads *joiner;

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
    done(false),joiner(new join_threads(threads))
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
      joiner->~join_threads();
    }

    template<typename FunctionType>
    void submit(FunctionType f, int i,SparseMatrix &a, SparseMatrix &m)
    {
      std::function<void()> my_f = std::bind(f, i, ref(a), ref(m));
      work_queue.push(my_f);
    }
};

void action(int i,SparseMatrix &origen,SparseMatrix &result)
{
  origen.row_mult(i,result);
}

void load(SparseMatrix &a,SparseMatrix &m, string source)
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
      a.set( v , r-1 , c-1 );
    }
  }
}

int main(int argc, char const *argv[])
{
  high_resolution_clock::time_point t1;
  high_resolution_clock::time_point t2;
  t1= high_resolution_clock::now();
  SparseMatrix a(NODES,NODES);
  SparseMatrix m(NODES,NODES);
  load(a, m, FILE);
  a.print();
  
  t2 = high_resolution_clock::now();
  cout << "matrix reading: " << duration_cast<microseconds>(t2 - t1).count() << " microseconds" << endl;
  //se comienza el ciclo
 
  for (int k=1; k <= LIMIT; k++)
  {
    thread_pool p;
    for (int i = 0; i < NODES; ++i)
    {
      p.submit(action,i,ref(a),ref(m));
    }
  }
  t1 = high_resolution_clock::now();
  cout << "Con. Diamond Operation " << LIMIT << " times: " << duration_cast<microseconds>(t1 - t2).count() << " microseconds" << endl;
  m.print();

  load(a, m, FILE);
  t2 = high_resolution_clock::now();
  for (int k=1; k <= 1; k++)
  {
    m.mult(a);
  }
  t1 = high_resolution_clock::now();
  cout << "Seq. Diamond Operation " << LIMIT << " times: " << duration_cast<microseconds>(t1 - t2).count() << " microseconds" << endl;
  m.print();
  return 0;
}
 