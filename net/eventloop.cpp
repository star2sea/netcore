#include "eventloop.h"
#include "channel.h"
#include "select.h"
#include "epoll.h"
#include "kqueue.h"
using namespace netcore;

EventLoop::EventLoop()
	:poller_(defaultPoller()),
	tid_(std::this_thread::get_id()),
	running_(false),
	wakeupSock_(createWakeupFd()),
	wakeupChannel_(new Channel(wakeupSock_.fd(), this))
{
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	wakeupSock_.close();
}

void EventLoop::loop()
{
	running_ = true;
	
	while (running_)
	{
		poller_->poll();
		std::vector<Func> funcs = pendingFuncs_.getAll();
		for (const Func & func : funcs)
		{
			func();
		}
	}
}

void EventLoop::quit()
{
	if (inOwnThread())
	{
		quitInLoop();
	}
	else
	{
		runInLoop(std::bind(&EventLoop::quitInLoop, this));
	}
}

void EventLoop::quitInLoop()
{
	running_ = false;
}

void EventLoop::updateChannel(Channel *channel)
{
	assertInOwnThread();
	poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel *channel)
{
	assertInOwnThread();
	poller_->removeChannel(channel);
}

void EventLoop::runInLoop(const Func &cb)
{
	if (inOwnThread())
	{
		cb();
	}
	else
	{
		pendingFuncs_.put(cb);
		wakeup();
	}
}

void EventLoop::wakeup()
{
	char one = ' ';
	ssize_t n = wakeupSock_.write(&one, sizeof one);
	if (n != sizeof one)
	{
		std::cout << "EventLoop::wakeup error" << std::endl;
	}
}

int EventLoop::createWakeupFd()
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	int len = sizeof(addr);
	int fd_1 = Socket::createSocket();
	if (::bind(SOCKET_HANDLE(fd_1), (struct sockaddr*)&addr, len) < 0)
	{
		std::cout << "EventLoop::createWakeupFd bind error" << std::endl;
		return -1;
	}
	if (::listen(SOCKET_HANDLE(fd_1), 5) < 0)
	{
		std::cout << "EventLoop::createWakeupFd listen error" << std::endl;
		return -1;
	}

	if (::connect(SOCKET_HANDLE(fd_1), (struct sockaddr*)&addr, len) < 0)
	{
		std::cout << "EventLoop::createWakeupFd connect error" << std::endl;
		return -1;
	}
	int fd_2 = ::accept(SOCKET_HANDLE(fd_1), (struct sockaddr*)&addr, &len);
	if (fd_2 < 0)
	{
		std::cout << "EventLoop::createWakeupFd accept error" << std::endl;
		return -1;
	}

	CLOSE_SOCKET(fd_1);
	return fd_2;
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
