#ifndef __KQUEUE_H
#define __KQUEUE_H

#include "poller.h"
#include <vector>
#include <sys/event.h>
#include <sys/time.h>

namespace netcore
{
	class KqueuePoller :public Poller
	{
	public:
		KqueuePoller(EventLoop *loop);
		~KqueuePoller() override;
		void updateChannel(Channel *) override;
		void removeChannel(Channel *) override;
		void poll() override;
	private:
		void handleActiveChannels() override;
	private:
		static const int kInitEvtSize = 16;
		int kqueuefd_;
		std::vector<struct kevent> eventlist_;
	};
}



#endif
