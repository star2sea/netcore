#ifndef __SERVER_H
#define __SERVER_H
#include "../utils/noncopyable.h"
#include "netaddr.h"
#include "eventloop.h"
#include "eventloopthread.h"
#include "acceptor.h"
#include "callback.h"
#include <string>
#include <vector>
namespace netcore
{
	class Server :NonCopyable
	{
	public:
		Server(EventLoop *loop, const NetAddr & addr, std::string &name);
		~Server();

		void start(int threadnum = 0);
		void stop();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void seMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }

	private:
		Acceptor * newAcceptor(EventLoop *loop);

	private:
		EventLoop *loop_;
		NetAddr addr_;
		std::string name_;
		Acceptor * acceptor_;
		bool running_;
		std::vector<EventLoopThread> threads_;

		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
	};
}

#endif