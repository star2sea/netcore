#include "buffer.h"
#include <assert.h>
using namespace netcore;

std::string Buffer::getAllAsString()
{
	std::string str(readBegin(), readAvailable());
	consume(readAvailable());
	return str;
}

void Buffer::consume(size_t n)
{
	assert(readAvailable() >= n);
	readIndex_ += n;
}

void Buffer::append(const char *buf, size_t n)
{
	ensureEnoughSpace(n);
	std::copy(buf, buf + n, writeBegin());
	writeIndex_ += n;
	
	if (writeAvailable() >= n)
	{
		
	}
	else if (vec_.size() - readAvailable() >= n)
	{
		std::copy(begin()+readIndex_, begin()+writeIndex_, begin());

	}
}

void Buffer::ensureEnoughSpace(size_t n)
{
	if (writeAvailable() < n)
	{
		if (vec_.size() - readAvailable() >= n)
		{
			size_t readable = readAvailable();
			std::copy(begin() + readIndex_, begin() + writeIndex_, begin());
			readIndex_ = 0;
			writeIndex_ = readable;
		}
		else
		{
			vec_.resize(writeIndex_ + n);
		}
	}
}