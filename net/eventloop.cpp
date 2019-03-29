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
	wakeupChannel_(new Channel(wakeupFd_[1], this))
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
	NetAddr serveraddr("127.0.0.1",20000);
	int len = static_cast<int>(sizeof(serveraddr.getAddr()));
	Socket sock(Socket::createSocket());
	sock.setReuseAddr(true);

	sock.bind(serveraddr);
	sock.listen(5);


	Socket sock_0(Socket::createSocket());
	wakeupFd_[0] = sock_0.fd();
	sock_0.connect(serveraddr);

	NetAddr peeraddr;
	wakeupFd_[1] = sock.accept(&peeraddr);

	sock.close();

	return wakeupFd_[0];
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
