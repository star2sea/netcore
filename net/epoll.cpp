#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "epoll.h"
#include "channel.h"
#include "../utils/logger.h"

using namespace netcore;

EpollPoller::EpollPoller(EventLoop *loop)
	:Poller(loop),
	epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
	eventlist_(kInitEvtSize)
{
	if (epollfd_ < 0) 
	{
		LOG_ERROR << "EpollPoller create epollfd failed";
	}
}

EpollPoller::~EpollPoller()
{
	::close(epollfd_);
}

void EpollPoller::updateChannel(Channel *channel)
{
	loop_->assertInOwnThread();
	int fd = channel->fd();
	if (channels_.find(fd) == channels_.end())
	{
		channels_[fd] = channel;
		update(EPOLL_CTL_ADD, channel);
	}
	else
	{
		update(EPOLL_CTL_MOD, channel);
	}
}

void EpollPoller::removeChannel(Channel *channel)
{
	loop_->assertInOwnThread();
	int fd = channel->fd();
	assert(channels_.find(fd) != channels_.end());
	if (channels_.find(fd) != channels_.end())
	{
		channels_.erase(fd);
		update(EPOLL_CTL_DEL, channel);
	}
}

void EpollPoller::update(int oper, Channel *channel)
{
	struct epoll_event epollEvt;
	::bzero(&epollEvt, sizeof epollEvt);

	int epollEvtFlags = 0;
	if (channel->isReading()) 
	{
		epollEvtFlags |= (EPOLLIN | EPOLLPRI);
	}
	if (channel->isWriting()) 
	{
		epollEvtFlags |= EPOLLOUT;
	}

	epollEvt.events = epollEvtFlags;
	epollEvt.data.ptr = static_cast<void *>(channel);

	auto fd = channel->fd();
	if (::epoll_ctl(epollfd_, oper, fd, &epollEvt) < 0) 
	{
		LOG_ERROR << "epoll_ctl error, operation = " << oper << " fd = " << fd;
	}
}

void EpollPoller::handleActiveChannels()
{
	for (int i = 0; i < activeNum_; ++i)
	{
		Channel * channel = static_cast<Channel*>(eventlist_[i].data.ptr);
		if (channel->isReading())
		{
			channel->handleReadable();
		}
		if (channel->isWriting())
		{
			channel->handleWritable();
		}
	}
}

void EpollPoller::poll()
{
	activeNum_ = ::epoll_wait(epollfd_, &*eventlist_.begin(), static_cast<int>(eventlist_.size()), -1);
	int savedErrno = errno;
	if (activeNum_ > 0) 
	{
		handleActiveChannels();
		if (static_cast<size_t>(activeNum_) == eventlist_.size()) 
		{
			eventlist_.resize(eventlist_.size() * 2);
		}
	}
	else if (activeNum_ < 0) 
	{
		if (savedErrno != EINTR)
		{
			LOG_ERROR << "epoll_wait failed, errno = " << savedErrno;
		}
	}
	else 
	{
		LOG_INFO << "Epoll nothing happended";
	}
	return;
}