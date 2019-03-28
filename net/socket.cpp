#include "socket.h"
#include <iostream>
using namespace netcore;

void Socket::setNonBlocking(int fd)
{
#ifdef _WIN32
	u_long ul = 1;
	ioctlsocket(SOCKET_HANDLE(fd), FIONBIO, &ul);
#else
	auto flags = fcntl(fd, F_GETFL, 0);
	flags |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
#endif
}

int Socket::createSocket()
{
	return SOCKET_FD(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
}

int Socket::createNonBlockingSocket()
{
	int sockfd = createSocket();
	setNonBlocking(sockfd);
	return sockfd;
}

void Socket::listen(int backlog)
{
	if (::listen(SOCKET_HANDLE(sockfd_), backlog) < 0)
	{
		std::cout << "Socket::listen error" << std::endl;
	}
}

void Socket::bind(const NetAddr & addr)
{
	if (::bind(SOCKET_HANDLE(sockfd_), addr.toSockAddr(), sizeof addr) < 0)
	{
		std::cout << "Socket::bind error" << std::endl;
	}
}

int Socket::accept(NetAddr *peeraddr)
{
	socklen_t addrlen = static_cast<socklen_t>(sizeof *peeraddr);
	int connfd = ::accept(SOCKET_HANDLE(sockfd_), peeraddr->toSockAddr(), &addrlen);
	if (connfd < 0)
	{
		std::cout << "Socket::accept error" << std::endl;
	}
	return connfd;
}

int Socket::connect(const NetAddr & addr)
{
	return ::connect(SOCKET_HANDLE(sockfd_), addr.toSockAddr(), sizeof addr);
}

ssize_t Socket::read(char *buf, size_t count)
{
#ifdef _WIN32
	return ::recv(SOCKET_HANDLE(sockfd_), buf, count, 0);
#else
	return ::read(SOCKET_HANDLE(sockfd_), buf, count);
#endif
}

ssize_t Socket::write(const char* buf, size_t len)
{
#ifdef _WIN32
	return ::send(SOCKET_HANDLE(sockfd_), buf, len, 0);
#else
	return ::write(SOCKET_HANDLE(sockfd_), buf, len);
#endif
}

void Socket::close()
{
	CLOSE_SOCKET(sockfd_);
}

void Socket::shutdown()
{
#ifdef _WIN32
	::shutdown(SOCKET_HANDLE(sockfd_), SD_BOTH);
#else
	::shutdown(SOCKET_HANDLE(sockfd_), SHUT_RDWR);
#endif
}

void Socket::shutdownRead()
{
#ifdef _WIN32
	::shutdown(SOCKET_HANDLE(sockfd_), SD_RECEIVE);
#else
	::shutdown(SOCKET_HANDLE(sockfd_), SHUT_RD);
#endif
}

void Socket::shutdownWrite()
{
#ifdef _WIN32
	::shutdown(SOCKET_HANDLE(sockfd_), SD_SEND);
#else
	::shutdown(SOCKET_HANDLE(sockfd_), SHUT_WR);
#endif
}

struct sockaddr_in Socket::getLocalAddr()
{
	struct sockaddr_in localaddr;
	memset(&localaddr, 0, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	::getsockname(SOCKET_HANDLE(sockfd_), (struct sockaddr *)&localaddr, &addrlen);
	return localaddr;
}

struct sockaddr_in Socket::getPeerAddr()
{
	struct sockaddr_in peeraddr;
	memset(&peeraddr, 0, sizeof peeraddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
	::getpeername(SOCKET_HANDLE(sockfd_), (struct sockaddr *)&peeraddr, &addrlen);
	return peeraddr;
}

void Socket::setTcpNoDelay(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(SOCKET_HANDLE(sockfd_), IPPROTO_TCP, TCP_NODELAY, (OPTVAL)(&optval), static_cast<socklen_t>(sizeof optval));
}

void Socket::setReuseAddr(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(SOCKET_HANDLE(sockfd_), SOL_SOCKET, SO_REUSEADDR, (OPTVAL)(&optval), static_cast<socklen_t>(sizeof optval));
}

void Socket::setReusePort(bool on)
{
#ifndef _WIN32
	int optval = on ? 1 : 0;
	::setsockopt(SOCKET_HANDLE(sockfd_), SOL_SOCKET, SO_REUSEPORT, (OPTVAL)(&optval), static_cast<socklen_t>(sizeof optval));
#endif
}

void Socket::setKeepAlive(bool on)
{
	int optval = on ? 1 : 0;
	::setsockopt(SOCKET_HANDLE(sockfd_), SOL_SOCKET, SO_KEEPALIVE, (OPTVAL)(&optval), static_cast<socklen_t>(sizeof optval));
}

int Socket::getSocketError()
{
	int optval = 0;
	socklen_t optlen = static_cast<socklen_t>(sizeof optval);

#ifdef _WIN32
	::getsockopt(SOCKET_HANDLE(sockfd_), SOL_SOCKET, SO_ERROR, (OPTVAL)&optval, &optlen);
	return optval;
#else
	if (::getsockopt(SOCKET_HANDLE(sockfd_), SOL_SOCKET, SO_ERROR, (OPTVAL)&optval, &optlen) < 0)
	{
		return errno;
	}
	else
	{
		return optval;
	}
#endif
}