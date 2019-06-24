#include "../net/server.h"
#include "../net/eventloop.h"
#include "../net/netaddr.h"
#include "../net/callback.h"
#include "../net/connection.h"
#include "../utils/logger.h"
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
		server_.setMessageCallback(std::bind(&ServerTest::onMessage, this, std::placeholders::_1, std::placeholders::_2));
	}
	~ServerTest() { server_.stop(); }
	void start(int threadnum = 0) { server_.start(threadnum); }
	void stop() { server_.stop(); }

	void onMessage(const ConnectionPtr &conn, Buffer &buf)
	{
		std::string msg = buf.getAllAsString();
		LOG_INFO << "new msg: " << msg;
	}

	void onConnection(const ConnectionPtr &conn)
	{
		if (conn->isConnected())
		{
			LOG_INFO << "new connction, peeraddr " << conn->getPeerAddr().toIpPort() << 
				" localaddr " << conn->getLocalAddr().toIpPort();
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
	NetAddr serveraddr("127.0.0.1", (short)33333);
	ServerTest st(&loop, serveraddr);
	st.start();
	loop.loop();

#ifdef _WIN32
	WSACleanup();
#endif
}