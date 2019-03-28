#ifndef __CONNECTION_H
#define __CONNECTION_H
#include "../utils/noncopyable.h"
#include <memory>
#include "eventloop.h"
#include "channel.h"
#include "callback.h"
#include "socket.h"
#include "buffer.h"
#include "netaddr.h"
namespace netcore
{
	class Connection :NonCopyable, std::enable_shared_from_this<Connection>
	{
	public:
		enum Estate
		{
			Connecting,
			Connected,
			Disconnected
		};

		Connection(EventLoop *loop, int connfd, NetAddr &peeraddr);
		~Connection();

        int fd() const {return sock_.fd();}
		void connectionEstablished();
		void connectionDestroyed();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }
		void setClosedCallback(const ClosedCallback & cb) { closedCallback_ = cb; }

		void handleReadable();
		void handleWritable();

		void send(const char* buf, size_t count);

	private:
		void sendInLoop(const char* buf, size_t count);
		void handleClosed();

	private:
		EventLoop * loop_;
		Socket sock_;
		Channel connChannel_;
		Estate state_;
		Buffer input_;
		Buffer output_;
		NetAddr peeraddr_;
		NetAddr localaddr_;

		MessageCallback messageCallback_;
		ConnectionCallback connectionCallback_;
		ClosedCallback closedCallback_;
	};
}

#endif
