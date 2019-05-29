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
#include  "codec/codec.h"
namespace netcore
{
	class Connection :NonCopyable, public std::enable_shared_from_this<Connection>
	{
	public:
		enum Estate
		{
			Connecting,
			Connected,
			Disconnecting,
			Disconnected
		};

		Connection(EventLoop *loop, int connfd, NetAddr &peeraddr);
		~Connection();

        int fd() const {return sock_.fd();}
		bool isConnected() const { return state_ == Connected; }

		void connectionEstablished();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }
		void setClosedCallback(const ClosedCallback & cb) { closedCallback_ = cb; }

		void handleReadable();
		void handleWritable();
		void handleClosed();

		void send(const char* buf, size_t count);
		void send(Buffer &buffer);
		void send(const std::string &str);

		void shutdown();
		void forceClose();

		const NetAddr & getPeerAddr() const { return peeraddr_; }
		const NetAddr & getLocalAddr() const { return localaddr_; }

		void setConnectionCodec(Codec * codec) { codec_ = codec; }

	private:
		void sendInLoop(const std::string & str);
		void shutdownInLoop();
		void forceCloseInLoop();

	private:
		EventLoop * loop_;
		Socket sock_;
		Channel connChannel_;
		Estate state_;
		Buffer input_;
		Buffer output_;
		NetAddr peeraddr_;
		NetAddr localaddr_;

		Codec *codec_;

		MessageCallback messageCallback_;
		ConnectionCallback connectionCallback_;
		ClosedCallback closedCallback_;
	};
}

#endif
