#include "connection.h"
#include "channel.h"
#include "../utils/timestamp.h"
using namespace netcore;

Connection::Connection(EventLoop *loop, int connfd, NetAddr& peeraddr)
	:loop_(loop),
	sock_(connfd),
	connChannel_(connfd, loop),
	state_(Connecting),
	input_(65536),
	output_(65536),
	peeraddr_(peeraddr),
	localaddr_(sock_.getLocalAddr())
{
	sock_.setTcpNoDelay(true);
	sock_.setKeepAlive(true);
}

Connection::~Connection()
{
	loop_->assertInOwnThread();
	sock_.close();
}

void Connection::connectionEstablished()
{
	loop_->assertInOwnThread();
	assert(state_ == Connecting);
	state_ = Connected;

	if (connectionCallback_)
	{
		connectionCallback_(shared_from_this());	
	}
		
	connChannel_.setReadableCallback(std::bind(&Connection::handleReadable, this));
	connChannel_.setWritableCallback(std::bind(&Connection::handleWritable, this));

	connChannel_.enableReading();
	if (output_.readAvailable() > 0)
	{
		connChannel_.enableWriting();
	}
}

void Connection::connectionDestroyed()
{
	loop_->assertInOwnThread();
	state_ = Disconnected;
    
    connChannel_.disableAll();
	connChannel_.remove();
    
    if (connectionCallback_)
        connectionCallback_(shared_from_this());
}

void Connection::handleReadable()
{
	loop_->assertInOwnThread();
	char buf[1024];
	ssize_t n = sock_.read(buf, 1024);

	if (n == 0)
	{
		handleClosed();
	}
	else if (n < 0)
	{
		
		handleClosed();
	}
	else
	{
		input_.append(buf, n);
		messageCallback_(shared_from_this(), input_);
	}

}

void Connection::handleWritable()
{
	loop_->assertInOwnThread();
	ssize_t n = sock_.write(output_.readBegin(), output_.readAvailable());
	if (n < 0)
	{
		handleClosed();
	}
	else
	{
		output_.consume(n);
		if (output_.readAvailable() == 0)
		{
			connChannel_.disableWriting();
			if (state_ == Disconnecting)
			{
				shutdownInLoop();
			}
		}
	}
}

void Connection::handleClosed()
{
	loop_->assertInOwnThread();
	closedCallback_(shared_from_this());
}

void Connection::shutdown()
{
	if (state_ == Connected)
	{
		state_ = Disconnecting;
		loop_->runInLoop(std::bind(&Connection::shutdownInLoop, shared_from_this()));
	}
}

void Connection::shutdownInLoop()
{
	loop_->assertInOwnThread();
	if (!connChannel_.isWriting())
	{
		sock_.shutdownWrite();
	}
}

void Connection::send(const char* buf, size_t count)
{
	if (loop_->inOwnThread())
	{
		sendInLoop(buf, count);
	}
	else
	{
		loop_->runInLoop(std::bind(&Connection::sendInLoop, this, buf, count));
	}
}

void Connection::send(Buffer &buffer)
{
	send(buffer.readBegin(), buffer.readAvailable());
}

void Connection::send(const std::string &str)
{
	if (str.empty())
		return;
	send(&*str.begin(), str.size());
}

void Connection::sendInLoop(const char* buf, size_t count)
{
	loop_->assertInOwnThread();

	if (output_.readAvailable() > 0)
	{
		assert(connChannel_.isWriting());
		output_.append(buf, count);
	}
	else
	{
		assert(!connChannel_.isWriting());
		ssize_t n = sock_.write(buf, count);
		if (n < 0)
		{
			handleClosed();
		}
		else if (n < count)
		{
			output_.append(buf + n, count - n);
			connChannel_.enableWriting();
		}
	}
}