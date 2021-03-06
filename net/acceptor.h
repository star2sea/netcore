#ifndef __ACCEPTOR_H
#define __ACCEPTOR_H
#include "../utils/noncopyable.h"
#include "netaddr.h"
#include "socket.h"
#include "callback.h"
#include "eventloop.h"
#include "callback.h"
#include "channel.h"
#include <map>
namespace netcore
{
	class Acceptor : NonCopyable
	{
	public:
		Acceptor(EventLoop * loop, const NetAddr &addr);
		~Acceptor();

		void startAccept();
		void stopAccept();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }

	private:
		void stopAcceptInLoop();
		void handleReadable();
		void removeConnection(const ConnectionPtr & conn);
		void onNewConnection(int connfd, NetAddr & peeraddr);

	private:
		EventLoop * loop_;
		NetAddr addr_;
		Socket sock_;
		Channel acceptChannel_;
		bool accepting_;

		std::map<int, ConnectionPtr> connections_;

		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
	};
}

#endif