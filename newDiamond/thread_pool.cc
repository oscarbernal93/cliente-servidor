#include "thread_pool.hh"

join_threads::~join_threads()
{
  for(unsigned long i=0;i<threads.size();++i)
  {
    if(threads[i].joinable())
      threads[i].join();
  }
}


void thread_pool::worker_thread()
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

thread_pool::thread_pool():done(false),joiner(threads)
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

thread_pool::~thread_pool()
{
	done=true;
}

template<typename FunctionType>
void thread_pool::submit(FunctionType f, int a, SparseMatrix &m)
{
	std::function<void()> my_f = std::bind(f, a, ref(m));
	work_queue.push(my_f);
}
