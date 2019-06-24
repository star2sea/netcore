#include "../../net/client.h"
#include "../../net/eventloop.h"
#include "../../net/netaddr.h"
#include "../../net/callback.h"
#include "../../net/connection.h"
#include "../../utils/logger.h"
#include "../../net/codec/protobuf/protobufcodec.h"
#include "../../net/codec/protorpc/protorpcchannel.h"
#include "rpcservice.pb.h"
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
		serveraddr_(addr),
		rpcChannel_(new ProtoRpcChannel()),
		stub_(rpcChannel_.get())
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
			rpcChannel_->connectionAttached(conn);
			conn->attachProtoRpcChannel(rpcChannel_);
			
			TestRequest testReq;
			testReq.set_testmsg("rpc test");
			stub_.TestRpc(NULL, &testReq, NULL, NULL);
			
		}
	}
private:
	Client client_;
	NetAddr serveraddr_;
	std::shared_ptr<ProtoRpcChannel> rpcChannel_;
	TestService::Stub stub_;
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