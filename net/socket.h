#ifndef __SOCK_H
#define __SOCK_H
#include "../utils/noncopyable.h"
#include "socketutils.h"

namespace netcore
{
	class Socket:NonCopyable
	{
		
	public:
		Socket(int sockfd) :sockfd_(sockfd) {}
		
		~Socket() {}

		void listen();
		void bind();
		int accept();
		int connect();

		ssize_t readv();
		ssize_t write();

		void close();
		void shutdown();
		void shutdownRead();
		void shutdownWrite();

		void setTcpNoDelay(bool on);
		void setReuseAddr(bool on);
		void setReusePort(bool on);
		void setKeepAlive(bool on);

	private:
		int sockfd_;
	};
}

#endif