#ifndef __CONNECTOR_H
#define __CONNECTOR_H
#include "../utils/noncopyable.h"
#include "eventloop.h"
#include "netaddr.h"
#include "callback.h"
#include "channel.h"
#include "socket.h"
#include "connection.h"
namespace netcore
{
	class Connector :NonCopyable, public std::enable_shared_from_this<Connector>
	{
	public:
		enum Estate
		{
			Connecting,
			Connected,
			Disconnected
		};
		Connector(EventLoop *loop);
		~Connector();

		void connect(const NetAddr & serveraddr);
		void disconnect();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }

	private:
		bool errnoIsWouldBlock(int err);
		void onConnected();
		void handleWritable();
		void handleClosed();

	private:
		EventLoop *loop_;
		Socket sock_;
		Channel connectorChannel_;
		Estate state_;
		ConnectionPtr connection_;

		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
	};
}

#endif
