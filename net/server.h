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
#include <memory>
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
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }

	private:
		EventLoop *loop_;
		NetAddr addr_;
		std::string name_;
		std::shared_ptr<Acceptor> acceptor_;
		bool running_;
		std::vector<std::unique_ptr<EventLoopThread>> threads_;

		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
	};
}

#endif
