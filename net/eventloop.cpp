#include "eventloop.h"
#include "channel.h"
#include "select.h"
#include "epoll.h"
#include "kqueue.h"
using namespace netcore;

EventLoop::EventLoop()
	:poller_(defaultPoller()),
	running_(false)
{

}

EventLoop::~EventLoop()
{
	delete poller_;

}

void EventLoop::loop()
{
	assertInOwnThread();

	running_ = true;
	
	while (running_)
	{
		poller_->poll();
	}
}

void EventLoop::quit()
{
	if (inOwnThread())
	{
		running_ = false;
	}
	else
	{
		// todo
	}
}

void EventLoop::updateChannel(Channel *channel)
{
	poller_->updateChannel(channel);
}

Poller * EventLoop::defaultPoller()
{
#ifdef USE_EPOLL
	return new EpollPoller(this);
#elif USE_KQUEUE
	return new KqueuePoller(this);
#else
	return new SelectPoller(this);
#endif

}
