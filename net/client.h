#ifndef __CLIENT_H
#define __CLIENT_H
#include <memory>
#include "../utils/noncopyable.h"
#include "eventloop.h"
#include "connector.h"
#include "callback.h"
#include "netaddr.h"
#include "eventloopthread.h"

namespace netcore
{
	class Client:NonCopyable
	{
	public:
		Client(EventLoop *loop);
		~Client();

		void start(const NetAddr & serveraddr);
		void stop();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }

	private:
		EventLoop *loop_;
		std::shared_ptr<Connector> connector_;
		bool connected_;

		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
	};
}

#endif