#ifndef __SELECT_H
#define __SELECT_H
#include "../utils/noncopyable.h"
#include "poller.h"
namespace netcore
{
	class SelectPoller :public Poller
	{
	public:
		SelectPoller(EventLoop *loop) :Poller(loop) {}
		void updateChannel(Channel *) override;
		void poll() override;
	};
}

#endif
