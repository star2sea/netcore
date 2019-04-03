#ifndef __EVENTLOOP_H
#define __EVENTLOOP_H
#include "../utils/noncopyable.h"
#include "../utils/blockingqueue.h"
#include "poller.h"
#include <thread>
#include <cassert>
#include <vector>
#include <functional>
#include "socket.h"
namespace netcore
{
    class Channel;
    class EventLoop :NonCopyable
	{
	public:
		typedef std::function<void()> Func;

		EventLoop();

		~EventLoop();
		
		void loop();

		void quit();

		void updateChannel(Channel *);
		void removeChannel(Channel *);

		void runInLoop(const Func &);

		bool inOwnThread() { return tid_ == std::this_thread::get_id(); } // TODO thread_±‰¡ø

		void assertInOwnThread() { assert(inOwnThread()); }

		//void createWakeupFdCallback(int wakeupFd);

	private:
		Poller * defaultPoller();
		void quitInLoop();
		int createWakeupFd();
		void wakeup();
		void onWakeup();

	private:
		std::unique_ptr<Poller> poller_;
		std::thread::id tid_;
        bool running_;
		Socket wakeupSock_[2];
		std::unique_ptr<Channel> wakeupChannel_;
		BlockingQueue<Func> pendingFuncs_;
		
	};
}

#endif
