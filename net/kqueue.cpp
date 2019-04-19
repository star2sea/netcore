#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "kqueue.h"
#include "channel.h"
#include "../utils/logger.h"

using namespace netcore;

KqueuePoller::KqueuePoller(EventLoop *loop)
	:Poller(loop), 
	kqueuefd_(kqueue()),
	eventlist_(2 * kInitEvtSize)
{
	if (kqueuefd_ < 0) {
		LOG_ERROR << "KqueuePoller create kqueuefd failed";
	}
}

KqueuePoller::~KqueuePoller()
{
	::close(kqueuefd_);
}


void KqueuePoller::updateChannel(Channel *channel)
{
	loop_->assertInOwnThread();

	struct kevent kevt[2];

	int fd = channel->fd();
	if (channels_.find(fd) == channels_.end())
	{
		channels_[fd] = channel;

		if (channel->isReading())
		{
			EV_SET(&kevt[0], fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, static_cast<void*>(channel));
		}
		else 
		{
			EV_SET(&kevt[0], fd, EVFILT_READ, EV_ADD | EV_DISABLE, 0, 0, static_cast<void*>(channel));
		}

		if (channel->isWriting())
		{
			EV_SET(&kevt[1], fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, static_cast<void*>(channel));
		}
		else 
		{
			EV_SET(&kevt[1], fd, EVFILT_WRITE, EV_ADD | EV_DISABLE, 0, 0, static_cast<void*>(channel));
		}
	}
	else
	{
		if (channel->isReading())
		{
			EV_SET(&kevt[0], fd, EVFILT_READ, EV_ENABLE, 0, 0, static_cast<void*>(channel));
		}
		else
		{
			EV_SET(&kevt[0], fd, EVFILT_READ, EV_DISABLE, 0, 0, static_cast<void*>(channel));
		}

		if (channel->isWriting())
		{
			EV_SET(&kevt[1], fd, EVFILT_WRITE, EV_ENABLE, 0, 0, static_cast<void*>(channel));
		}
		else 
		{
			EV_SET(&kevt[1], fd, EVFILT_WRITE, EV_DISABLE, 0, 0, static_cast<void*>(channel));
		}
	}

	if (auto ret = kevent(kqueuefd_, kevt, 2, NULL, 0, NULL) == -1)
		LOG_ERROR << "kevent register failed";
}

void KqueuePoller::removeChannel(Channel *channel)
{
	loop_->assertInOwnThread();
	struct kevent kevt[2];
	int fd = channel->fd();
	assert(channels_.find(fd) != channels_.end());
	if (channels_.find(fd) != channels_.end())
	{
		channels_.erase(fd);

		EV_SET(&kevt[0], fd, EVFILT_READ, EV_DELETE, 0, 0, static_cast<void*>(channel));
		EV_SET(&kevt[1], fd, EVFILT_WRITE, EV_DELETE, 0, 0, static_cast<void*>(channel));
		
		if (auto ret = kevent(kqueuefd_, kevt, 2, NULL, 0, NULL) == -1)
			LOG_ERROR << "kevent register failed";
	}	
}

void KqueuePoller::handleActiveChannels()
{
	for (int i = 0; i < activeNum_; ++i) {
		Channel * channel = static_cast<Channel*>(eventlist_[i].udata);
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

void KqueuePoller::poll()
{
	activeNum_ = kevent(kqueuefd_, NULL, 0, &*eventlist_.begin(), eventlist_.size(), NULL);

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
			LOG_ERROR << "kevent failed, errno = " << savedErrno;
		}
	}
	else 
	{
		LOG_INFO << "Kqueue nothing happended";
	}
}
