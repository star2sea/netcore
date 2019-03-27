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

int Socket::accept()
{

}

int Socket::connect()
{

}

ssize_t Socket::readv()
{

}

ssize_t Socket::write()
{

}

void Socket::close()
{

}

void Socket::shutdown()
{

}

void Socket::shutdownRead()
{

}

void Socket::shutdownWrite()
{

}

void Socket::setTcpNoDelay(bool on)
{

}

void Socket::setReuseAddr(bool on)
{

}

void Socket::setReusePort(bool on)
{

}

void Socket::setKeepAlive(bool on)
{

}