#include "../../net/server.h"
#include "../../net/eventloop.h"
#include "../../net/netaddr.h"
#include "../../net/callback.h"
#include "../../net/connection.h"
#include "../../utils/logger.h"
#include "../../net/codec/protobuf/protobufcodec.h"
#include "../../net/codec/protobuf/protorpcchannel.h"
#include "rpcservice.pb.h"
#include <functional>

#ifdef _WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
using namespace netcore;

class ServerTest
{
public:
	ServerTest(EventLoop *loop, const NetAddr &addr)
		:server_(loop, addr, std::string("servertest"))
	{
		server_.setConnectionCallback(std::bind(&ServerTest::onConnection, this, std::placeholders::_1));
	}
	~ServerTest() { server_.stop(); }
	void start(int threadnum = 0) { server_.start(threadnum); }
	void stop() { server_.stop(); }

	

	void onConnection(const ConnectionPtr &conn)
	{
		if (conn->isConnected())
		{
			LOG_INFO << "new connction, peeraddr " << conn->getPeerAddr().toIpPort() << 
				" localaddr " << conn->getLocalAddr().toIpPort();

			std::shared_ptr<ProtoRpcChannel> ptr = conn->attachNewProtoRpcChannel();
			ptr->registerRpcService(); // TODO
		}
		else
		{
			LOG_INFO << "connction disconnected, peeraddr " << conn->getPeerAddr().toIpPort() << 
				" localaddr " << conn->getLocalAddr().toIpPort();
		}
	}
private:
	Server server_;
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
	NetAddr serveraddr("127.0.0.1", 30000);
	ServerTest st(&loop, serveraddr);
	st.start();
	loop.loop();

#ifdef _WIN32
	WSACleanup();
#endif
}