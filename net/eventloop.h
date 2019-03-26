#ifndef __EVENTLOOP_H
#define __EVENTLOOP_H
#include "../utils/noncopyable.h"
#include "poller.h"
#include <thread>
#include <cassert>
namespace netcore
{
    class Channel;
    class EventLoop :NonCopyable
	{
	public:
		EventLoop();

		~EventLoop();
		
		void loop();

		void quit();

		void updateChannel(Channel *);

		bool inOwnThread() { return tid_ == std::this_thread::get_id(); } // TODO thread_±‰¡ø

		void assertInOwnThread() { assert(inOwnThread()); }

	private:
		Poller * defaultPoller();

	private:
		Poller * poller_;
		bool running_;
		std::thread::id tid_;
	};
}

#endif
