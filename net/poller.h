#ifndef __POLLER_H
#define __POLLER_H
#include "../utils/noncopyable.h"
#include <iostream>
#include <vector>
namespace netcore
{
	class EventLoop;
	class Channel;

	class Poller :NonCopyable
	{
	public:
		Poller(EventLoop * loop) : loop_(loop) {}
		virtual void updateChannel(Channel *) = 0;
		virtual void poll() = 0;
	private:
		EventLoop * loop_;
		std::vector<Channel *> channels_;
	};
}

#endif
