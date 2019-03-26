#include "blockingqueue.h"
using namespace netcore;

template <typename T>
T BlockingQueue<T>::get()
{
	std::unique_lock lock(mutex_);
	cond_.wait(lock, [this] {return !this->queue_.empty(); })
	T t = queue_.front();
	queue_.pop();
	return t;
}

template <typename T>
void BlockingQueue<T>::put(const T &t)
{
	std::unique_lock lock(mutex_);
	queue_.push(t);
	cond_.notify_one();
}

template <typename T>
bool BlockingQueue<T>::tryGet(T &t)
{
	std::unique_lock lock(mutex_);
	if (queue_.empty())
		return false;
	t = queue_.front();
	queue_.pop();
	return true;

}