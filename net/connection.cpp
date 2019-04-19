#include "connection.h"
#include "channel.h"
#include "../utils/timestamp.h"
#include "../utils/logger.h"
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
	sock_.setReuseAddr(true);
	sock_.setKeepAlive(true);
}

Connection::~Connection()
{
	LOG_TRACE << "Connection::~Connection fd = " << fd();
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
	connChannel_.tie(shared_from_this());

	connChannel_.enableReading();
	if (output_.readAvailable() > 0)
	{
		connChannel_.enableWriting();
	}
}

void Connection::handleReadable()
{
	loop_->assertInOwnThread();

#ifdef _WIN32
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
#else
	char buf[65536];
	struct iovec vec[2];
	const size_t writable = input_.writeAvailable();
	vec[0].iov_base = input_.writeBegin();
	vec[0].iov_len = writable;
	vec[1].iov_base = buf;
	vec[1].iov_len = sizeof buf;
	const int iovcnt = (writable < sizeof buf) ? 2 : 1;
	ssize_t n = sock_.readv(vec, iovcnt);
	if (n == 0)
	{
		handleClosed();
	}
	else if (n < 0)
	{
		handleClosed();
	}
	else if (size_t(n) <= writable)
	{
		input_.hasWritten(n);
		messageCallback_(shared_from_this(), input_);
	}
	else
	{
		input_.hasWritten(writable);
		input_.append(buf, n - writable);
		messageCallback_(shared_from_this(), input_);
	}
#endif
}

void Connection::handleWritable()
{
	loop_->assertInOwnThread();
	ssize_t n = sock_.write(output_.readBegin(), output_.readAvailable());
	if (n < 0)
	{
		LOG_ERROR << "Connection::handleWritable write error, fd = " << fd() << " errno = " << ERRNO;
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
				sock_.shutdownWrite();
			}
		}
	}
}

void Connection::handleClosed()
{
	loop_->assertInOwnThread();
	LOG_TRACE << "Connection::handleClosed: fd = " << fd() << " state = " << state_;
	assert(state_ == Connected || state_ == Disconnecting);
	state_ = Disconnected;
	connChannel_.disableAll();
	connChannel_.remove();
	ConnectionPtr guard(shared_from_this());
	if (connectionCallback_)
		connectionCallback_(guard);
	closedCallback_(guard);
}

void Connection::shutdown()
{
	if (state_ == Connected)
	{
		if (loop_->inOwnThread())
			shutdownInLoop();
		else
			loop_->runInLoop(std::bind(&Connection::shutdownInLoop, shared_from_this()));
	}
}

void Connection::shutdownInLoop()
{
	loop_->assertInOwnThread();

	if (state_ == Connected)
	{
		state_ = Disconnecting;

		if (!connChannel_.isWriting())
		{
			sock_.shutdownWrite();
		}
	}
}

void Connection::forceClose()
{
	if (loop_->inOwnThread())
	{
		forceCloseInLoop();
	}
	else
	{
		loop_->runInLoop(std::bind(&Connection::forceCloseInLoop, shared_from_this()));
	}
}

void Connection::forceCloseInLoop()
{
	loop_->assertInOwnThread();
	if (state_ == Connected)
	{
		state_ = Disconnected;
		connChannel_.disableAll();
		connChannel_.remove();

		if (connectionCallback_)
			connectionCallback_(shared_from_this());
	}
}

void Connection::send(const char* buf, size_t count)
{
	if (loop_->inOwnThread())
	{
		sendInLoop(std::string(buf, count));
	}
	else
	{
		loop_->runInLoop(std::bind(&Connection::sendInLoop, this, std::string(buf, count)));
	}
}

void Connection::send(Buffer &buffer)
{
	if (loop_->inOwnThread())
	{
		sendInLoop(buffer.getAllAsString());
	}
	else
	{
		loop_->runInLoop(std::bind(&Connection::sendInLoop, this, buffer.getAllAsString()));
	}
}

void Connection::send(const std::string &str)
{
	if (loop_->inOwnThread())
	{
		sendInLoop(str);
	}
	else
	{
		loop_->runInLoop(std::bind(&Connection::sendInLoop, this, str));
	}
}

void Connection::sendInLoop(const std::string & str)
{
	loop_->assertInOwnThread();

	if (str.empty())
		return;

	if (output_.readAvailable() > 0)
	{
		assert(connChannel_.isWriting());
		output_.append(str);
	}
	else
	{
		assert(!connChannel_.isWriting());
		
		const char * buf = &*str.begin();
		size_t count = str.size();
		ssize_t n = sock_.write(buf, count);
		if (n < 0)
		{
			LOG_ERROR << "Connection::SendInloop write error, fd = " << fd() << " errno = " << ERRNO;
			handleClosed();
		}
		else if (n < count)
		{
			output_.append(buf + n, count - n);
			connChannel_.enableWriting();
		}
	}
}