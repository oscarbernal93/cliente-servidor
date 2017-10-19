#ifndef _SAFEQUEUE_HH_
  #define _SAFEQUEUE_HH_

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <utility>
#include <unistd.h>

#define ZERO 0

using namespace std;

template <typename T>
class threadsafe_queue
{
    protected:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;

    public:
	threadsafe_queue();
	threadsafe_queue(threadsafe_queue const& other);
	void push(T new_value);
	void wait_and_pop(T& value);
	std::shared_ptr<T> wait_and_pop();
	bool try_pop(T& value);
	std::shared_ptr<T> try_pop();
	bool empty() const;
};

#endif
