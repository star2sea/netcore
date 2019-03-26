#ifndef __BLOCKING_QUEUE_H
#define __BLOCKING_QUEUE_H
#include <queue>
#include <condition_variable>
#include <mutex>
#include "noncopyable.h"

namespace netcore
{
	template<typename T>
	class BlockingQueue:NonCopyable
	{
	public:
		T get()
		{
			std::unique_lock<std::mutex> lock(mutex_);
			cond_.wait(lock, [this] {return !this->queue_.empty(); });
				T t = queue_.front();
			queue_.pop();
			return t;
		}

		void put(const T &t) 
		{
			std::unique_lock<std::mutex> lock(mutex_);
			queue_.push(t);
			cond_.notify_one();
		}

		bool tryGet(T &t) 
		{
			std::unique_lock<std::mutex> lock(mutex_);
			if (queue_.empty())
				return false;
			t = queue_.front();
			queue_.pop();
			return true;
		}

	private:
		std::mutex mutex_;
		std::condition_variable cond_;
		std::queue<T> queue_;
	};
}

#endif