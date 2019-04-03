#ifndef __SELECT_H
#define __SELECT_H

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/select.h>
#endif

#include "../utils/noncopyable.h"
#include "poller.h"

namespace netcore
{
	class SelectPoller :public Poller
	{
	public:
		SelectPoller(EventLoop *loop) :Poller(loop) {}
		~SelectPoller() override {}
		void updateChannel(Channel *) override;
		void removeChannel(Channel *) override;
		void poll() override;
	private:
		void handleActiveChannels() override;
	private:
		int maxfd_;
		fd_set rfds_;
		fd_set wfds_;
	};
}

#endif
