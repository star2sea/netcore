#include "eventloop.h"
#include "channel.h"
#include <stdlib.h>
#include "../utils/logger.h"
#include "../utils/threadpool.h"
#include <sstream>

#ifdef USE_EPOLL
#include "epoll.h"
#elif USE_KQUEUE
#include "kqueue.h"
#else
#include "select.h"
#endif

#ifndef _WIN32
#include <signal.h>
namespace 
{
	class IgnoreSigPipe
	{
	public:
		IgnoreSigPipe()
		{
			::signal(SIGPIPE, SIG_IGN);
		}
	};
	IgnoreSigPipe initObj;
}
#endif

using namespace netcore;

EventLoop::EventLoop()
	:poller_(defaultPoller()),
	tid_(std::this_thread::get_id()),
	running_(false)
{
	createWakeupFd();
	wakeupChannel_ = std::unique_ptr<Channel>(new Channel(wakeupSock_[1].fd(), this));
	wakeupChannel_->setReadableCallback(std::bind(&EventLoop::onWakeup, this));
	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	wakeupSock_[0].close();
	wakeupSock_[1].close();
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

void EventLoop::onWakeup()
{
	char one[1];
	ssize_t n = wakeupSock_[1].read(one, sizeof one);
	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::onWakeup error, errno = " << ERRNO;
	}
	else
	{
		LOG_TRACE << "EventLoop onWakeup";
	}
}

void EventLoop::wakeup()
{
	char one = ' ';
	ssize_t n = wakeupSock_[0].write(&one, sizeof one);
	if (n != sizeof one)
	{
		LOG_ERROR << "EventLoop::wakeup error, errno = " << ERRNO;
	}
}

int EventLoop::createWakeupFd()
{
	NetAddr serveraddr("127.0.0.1", 0);
	int len = static_cast<int>(sizeof(serveraddr.getAddr()));
	Socket serv(Socket::createSocket());

	serv.bind(serveraddr);
	serv.listen(1);

	serveraddr = serv.getLocalAddr();

	wakeupSock_[0] = Socket::createSocket();

	wakeupSock_[0].connect(serveraddr);

	wakeupSock_[1] = serv.accept(NULL);

	Socket::setNonBlocking(wakeupSock_[0].fd());
	Socket::setNonBlocking(wakeupSock_[1].fd());

	serv.close();

	return 0;
}

//int EventLoop::createWakeupFd()
//{
//	ThreadPool pool(2);
//	pool.start();
//
//
//	std::ostringstream oss;
//	oss << std::this_thread::get_id();
//	std::string stid = oss.str();
//	unsigned long long tid = std::stoull(stid);
//
//	short port = (tid % 1000) + 10000; // TODO
//	printf("port = %d\n", port);
//	pool.run([this, port]() 
//	{
//		NetAddr serveraddr("127.0.0.1", port);
//		int len = static_cast<int>(sizeof(serveraddr.getAddr()));
//		Socket sock(Socket::createSocket());
//		sock.setReuseAddr(true);
//		sock.setReusePort(true);
//		sock.bind(serveraddr);
//		sock.listen(1);
//
//		NetAddr peeraddr;
//		int connfd = sock.accept(&peeraddr);
//		if (connfd < 0)
//		{
//			LOG_ERROR << "createWakeupFd Failed, errno = " << ERRNO;
//		}
//		else
//		{
//			LOG_ERROR << "createWakeupFd Succeed, connfd = " << connfd;
//			sock.close();
//			this->runInLoop(std::bind(&EventLoop::createWakeupFdCallback, this, connfd));
//		}
//	});
//
//	pool.run([this, port]()
//	{
//		printf("run thread to connect\n");
//		NetAddr serveraddr("127.0.0.1", port);
//		int ret = this->wakeupSock_.connect(serveraddr);
//		if (ret < 0)
//		{
//			LOG_ERROR << "connect error " << ERRNO;
//		}
//	});
//	return 0;
//}

//void EventLoop::createWakeupFdCallback(int wakeupFd)
//{
//	assertInOwnThread();
//	wakeupChannel_ = std::unique_ptr<Channel>(new Channel(wakeupFd, this));
//	wakeupChannel_->enableReading();
//}

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
