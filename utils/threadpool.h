#ifndef __THREADPOOL_H
#define __THREADPOOL_H
#include <functional>
#include "blockingqueue.h"
#include "noncopyable.h"
#include <iostream>

namespace netcore
{
	class ThreadPool:NonCopyable
	{
	public:
		ThreadPool(int threadnum) : threadnum_(threadnum) {}
		~ThreadPool();

		void start();
		void stop();
		void run(std::function<void()> &task);

	private:
		int threadnum_;
		BlockingQueue queue_;
	};
}

#endif