#include "threadpool.h"
using namespace netcore;

ThreadPool::~ThreadPool()
{
	stop();
}

void ThreadPool::start()
{
	stop_ = false;
	for (auto i = 0; i < threadnum_; ++i)
	{
		threads_.emplace_back(std::thread([this]{
			while (!stop_)
			{
				Task task = this->queue_.get();
				task();
			}
		}));
	}
}

void ThreadPool::stop()
{
	stop_ = true;

	for (auto i = 0; i < threads_.size(); ++i)
	{
		Task empty = []() {};
		run(empty);
	}

	for (auto i = 0; i < threads_.size(); ++i)
	{
		threads_[i].join();
	}
}

void ThreadPool::run(Task &task)
{
	queue_.put(task);
}