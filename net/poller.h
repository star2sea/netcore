#ifndef __POLLER_H
#define __POLLER_H
#include "../utils/noncopyable.h"
#include <iostream>
#include <map>
#include <set>
namespace netcore
{
	class EventLoop;
	class Channel;

	class Poller :NonCopyable
	{
	public:
		Poller(EventLoop * loop) : loop_(loop) {}
        virtual ~Poller() = default;
		virtual void updateChannel(Channel *) = 0;
		virtual void removeChannel(Channel *) = 0;
		virtual void poll() = 0;

	protected:
		virtual void handleActiveChannels() = 0;

	protected:
		EventLoop * loop_;
		int activeNum_;
		std::map<int, Channel*> channels_;
		std::set<Channel *> changes_;
	};
}

#endif
