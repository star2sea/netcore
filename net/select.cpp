#include "select.h"
#include "channel.h"
#include <stdio.h>
using namespace netcore;

void SelectPoller::updateChannel(Channel *channel)
{
	loop_->assertInOwnThread();
	changes_.insert(channel);
}

void SelectPoller::removeChannel(Channel *channel)
{
	loop_->assertInOwnThread();
	
	int fd = channel->fd();
	if (channels_.find(fd) != channels_.end())
		channels_.erase(fd);

	if (changes_.find(channel) != changes_.end())
		changes_.erase(channel);
}

void SelectPoller::poll()
{
	loop_->assertInOwnThread();

	maxfd_ = 0;

	FD_ZERO(&rfds_);
	FD_ZERO(&wfds_);

	struct timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;

	handleChangeChannels();

	for (auto iter = channels_.cbegin(); iter != channels_.cend(); ++iter)
	{
		Channel * channel = iter->second;
		int fd = channel->fd();

		if (channel->isReading())
		{
			FD_SET(SOCKET_HANDLE(fd), &rfds_);
			maxfd_ = maxfd_ > fd ? maxfd_ : fd;
		}

		if (channel->isWriting())
		{
			FD_SET(SOCKET_HANDLE(fd), &wfds_);
			maxfd_ = maxfd_ > fd ? maxfd_ : fd;
		}
	}
		
	activeNum_ = select(maxfd_, &rfds_, &wfds_, NULL, NULL);

	if (activeNum_ > 0)
	{
		//std::cout << "Select something happened, activeNum = " << activeNum_ << std::endl;
		handleActiveChannels();
	}
	else if (activeNum_ == 0)
	{
		std::cout << "Select nothing happened" << std::endl;
	}
	else
	{
		std::cout << "Select error" << ERRNO << std::endl; // errno == EINTR
	}
}

// 如果更新的时候，不删除，貌似没有必要使用changes_ todo
void SelectPoller::handleChangeChannels()
{
	for (auto iter = changes_.cbegin(); iter != changes_.cend(); ++iter)
	{
		Channel * channel = *iter;
		int fd = channel->fd();
		//bool nonEvt = channel->isNonEvent();
		if (channels_.find(fd) == channels_.end())
		{
			//if (!nonEvt)
			channels_[fd] = channel;
		}
		/*else
		{
			if (nonEvt)
			{
				channels_.erase(fd);
			}
		}*/
	}
	changes_.clear();
}

void SelectPoller::handleActiveChannels()
{
	std::set<int> fds;
	for (auto iter = channels_.cbegin(); iter != channels_.cend(); ++iter)
	{
		Channel * channel = iter->second;
		int fd = channel->fd();

		if (FD_ISSET(SOCKET_HANDLE(fd), &rfds_)) 
		{
			channel->handleReadable();
			fds.insert(fd);
		}

		if (FD_ISSET(SOCKET_HANDLE(fd), &wfds_))
		{
			channel->handleWritable();
			fds.insert(fd);
		}

		if (fds.size() == activeNum_)
			break;
	}
}