#ifndef __EPOLL_H
#define __EPOLL_H

#include "poller.h"
#include <vector>
#include <sys/epoll.h>

namespace netcore
{
	class EpollPoller : public Poller
	{
	public:
		EpollPoller(EventLoop *loop);
		~EpollPoller() override;
		void updateChannel(Channel *) override;
		void removeChannel(Channel *) override;
		void poll() override;
	private:
		void update(int, Channel *);
		void handleActiveChannels() override;
	private:
		int epollfd_;
		static const int kInitEvtSize = 16;
		std::vector<struct epoll_event> eventlist_;
	};
}


#endif