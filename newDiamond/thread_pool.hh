#ifndef _THREAD_POOL_HH_
  #define _THREAD_POOL_HH_

#include "sparse_matrix.hh"
#include "safe_queue.hh"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>
#include <math.h>
#include <iostream>
#include <string>

#define ZERO 0

using namespace std;

class join_threads
{
    protected:
	std::vector<std::thread>& threads;

    public:
	explicit join_threads(std::vector<std::thread>& threads_):threads(threads_){};
	~join_threads();
};

class thread_pool
{
    protected:
	std::atomic_bool done;
	threadsafe_queue<std::function<void()> > work_queue;
	std::vector<std::thread> threads;
	join_threads joiner;
	void worker_thread();

    public:
	thread_pool();
	~thread_pool();
	template<typename FunctionType>
	void submit(FunctionType f, int a, SparseMatrix &m);
};

#endif
