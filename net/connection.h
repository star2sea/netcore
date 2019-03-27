#ifndef __CONNECTION_H
#define __CONNECTION_H
#include "../utils/noncopyable.h"
#include <memory>
#include "eventloop.h"
#include "callback.h"
#include "socket.h"
namespace netcore
{
	class Channel;
	class Connection :NonCopyable, std::enable_shared_from_this<Connection>
	{
	public:
		enum Estate
		{
			Connecting,
			Connected,
			Disconnected
		};

		Connection(EventLoop *loop, int connfd);
		~Connection();

        int fd() const {return sock_.fd();}
		void connectionEstablished();
		void connectionDestroyed();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }
		void setClosedCallback(const ClosedCallback & cb) { closedCallback_ = cb; }

		void handleReadable();
		void handleWritable();

		void send();

	private:
		EventLoop * loop_;
		Socket sock_;
		Channel *connChannel_;
		Estate state_;

		MessageCallback messageCallback_;
		ConnectionCallback connectionCallback_;
		ClosedCallback closedCallback_;
	};
}

#endif
