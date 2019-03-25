#ifndef __THREADPOOL_H
#define __THREADPOOL_H
#include <functional>
#include "blockingqueue.h"
#include "noncopyable.h"
#include <iostream>
#include <vector>
#include <thread>

namespace netcore
{
	class ThreadPool:NonCopyable
	{
	public:
		typedef std::function<void()> Task;
		ThreadPool(int threadnum) : threadnum_(threadnum), stop_(true) {}
		~ThreadPool();

		void start();
		void stop();
		void run(Task &task);

	private:
		int threadnum_;
		bool stop_;
		std::vector<std::thread> threads_;
		BlockingQueue<Task> queue_;
	};
}

#endif