#ifndef __SOCK_H
#define __SOCK_H

#ifdef _WIN32
#include <Winsock2.h>
#include <io.h>
#include <Ws2def.h> 
#include <ws2ipdef.h>
#include <WS2tcpip.h> //inet_pton
#else
#include <sys/socket.h>
#include <netinet/in.h>  // IPPROTO_TCP
#include <netinet/tcp.h> // TCP_NODELAY
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#endif

#include <fcntl.h>
#include "../utils/noncopyable.h"
#include "netaddr.h"

#ifdef _WIN32
#define SOCKET_FD(handle) _open_osfhandle(handle, 0)
#define SOCKET_HANDLE(fd) _get_osfhandle(fd)
#define CLOSE_SOCKET(fd) closesocket(SOCKET_HANDLE(fd))
#define ERRNO WSAGetLastError()
typedef long long ssize_t;
typedef int socklen_t;
typedef char * OPTVAL;
#else
#define SOCKET_FD(fd) fd
#define SOCKET_HANDLE(fd) fd
#define CLOSE_SOCKET(fd) close(fd)
#define ERRNO errno
typedef void * OPTVAL;
#endif

namespace netcore
{
	class Socket
	{
		
	public:
		Socket() = default;
		Socket(int sockfd) :sockfd_(sockfd) {}
		
		~Socket() {}

		int fd() const { return sockfd_; }

		void listen(int backlog);
		void bind(const NetAddr & addr);
		int accept(NetAddr *peeraddr);
		int connect(const NetAddr & addr);

		ssize_t read(char *buf, size_t count);
		ssize_t write(const char* buf, size_t len);

		void close();
		void shutdown();
		void shutdownRead();
		void shutdownWrite();

		void setTcpNoDelay(bool on);
		void setReuseAddr(bool on);
		void setReusePort(bool on);
		void setKeepAlive(bool on);

		struct sockaddr_in getLocalAddr();
		struct sockaddr_in getPeerAddr();

		int getSocketError();

		static void setNonBlocking(int fd);
		static int createSocket();
		static int createNonBlockingSocket();

	private:
		int sockfd_;
	};
}

#endif