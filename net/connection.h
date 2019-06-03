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
	class ProtoRpcChannel;

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
		typedef std::shared_ptr<ProtoRpcChannel> ProtoRpcChannelPtr;

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

		ProtoRpcChannelPtr attachNewProtoRpcChannel();

		void attachProtoRpcChannel(const ProtoRpcChannelPtr &);

		template <class CodecT>
		void setConnectionCodec(Codec * codec) 
		{ 
			codec_ = codec; 
			CodecT *realcodec = static_cast<CodecT *>(codec);
			setMessageCallback(std::bind(&CodecT::onMessage, realcodec, std::placeholders::_1, std::placeholders::_2));
		}

		Codec * getConnectionCodec() { return codec_; }

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
		ProtoRpcChannelPtr protoRpcChannel_;

		MessageCallback messageCallback_;
		ConnectionCallback connectionCallback_;
		ClosedCallback closedCallback_;
	};
}

#endif
