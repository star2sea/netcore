#include "threadpool.h"
#include <chrono>
using namespace netcore;

ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::start()
{
	stop_ = false;
	for (int i = 0; i < threadnum_; ++i)
	{
		threads_.emplace_back(std::thread([&]{
			while (!stop_)
			{
				Task task = queue_.get();
				task();
			}
		}));
	}
}

void ThreadPool::stop()
{
	if (stop_)
		return;

	stop_ = true;

	for (size_t i = 0; i < threads_.size(); ++i)
	{
		Task empty = []() {};
		run(empty);
	}

	for (size_t i = 0; i < threads_.size(); ++i)
	{
		threads_[i].join();
	}

	threads_.clear();

}

void ThreadPool::run(const Task &task)
{
	queue_.put(task);
}