#include "../../net/client.h"
#include "../../net/eventloop.h"
#include "../../net/netaddr.h"
#include "../../net/callback.h"
#include "../../net/connection.h"
#include "../../utils/logger.h"
#include "../../net/codec/protobuf/protobufcodec.h"
#include "test.pb.h"
#include <functional>

#ifdef _WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
using namespace netcore;

class ClientTest
{
public:
	ClientTest(EventLoop *loop, const NetAddr &addr)
		:client_(loop),
		serveraddr_(addr)
	{
		client_.setConnectionCallback(std::bind(&ClientTest::onConnection, this, std::placeholders::_1));
	}
	~ClientTest() { client_.stop(); }
	void start() { client_.start(serveraddr_); }
	void stop() { client_.stop(); }

	void onMessage(const ConnectionPtr &conn, const ProtoMsgPtr & msg)
	{
		LOG_INFO << "new msg: " << msg->GetTypeName();
	}

	void onConnection(const ConnectionPtr &conn)
	{
		if (conn->isConnected())
		{
			ProtobufCodec *codec = new ProtobufCodec();
			codec->setMessageCallback(std::bind(&ClientTest::onMessage, this, std::placeholders::_1, std::placeholders::_2));
			conn->setConnectionCodec<ProtobufCodec>(static_cast<Codec*>(codec));
			conn->setMessageCallback(std::bind(&ProtobufCodec::onMessage, codec, std::placeholders::_1, std::placeholders::_2));
					
			Test test;
			test.set_id(1);
			test.set_tester("qbh");

			Buffer buf;
			codec->fillEmptyBuffer(&buf, &test);
			conn->send(buf);
		}
	}
private:
	Client client_;
	NetAddr serveraddr_;
};

int main()
{
#ifdef _WIN32
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
#endif
	EventLoop loop;
	NetAddr serveraddr("127.0.0.1", (short)33333);
	ClientTest ct(&loop, serveraddr);
	ct.start();
	loop.loop();
#ifdef _WIN32
	WSACleanup();
#endif
}