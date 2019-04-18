#include "channel.h"
using namespace netcore;

void Channel::reallocate(int fd)
{
	if (fd_ == fd)
		return;
	disableAll();
	remove();
	fd_ = fd;
}

void Channel::handleReadable()
{
	std::shared_ptr<void> guard;
	if (tied_)
	{
		guard = tie_.lock();
		if (guard)
		{
			handleReadableWithGuard();
		}
	}
	else
	{
		handleReadableWithGuard();
	}
}

void Channel::handleWritable()
{
	std::shared_ptr<void> guard;
	if (tied_)
	{
		guard = tie_.lock();
		if (guard)
		{
			handleWritableWithGuard();
		}
	}
	else
	{
		handleWritableWithGuard();
	}
}