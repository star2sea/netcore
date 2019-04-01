#include "httpserver.h"
using namespace netcore;

#ifdef _WIN32
#pragma comment(lib,"ws2_32.lib")
#endif
using namespace netcore;

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
	NetAddr serveraddr(22222);
	HttpServer httpserver(&loop, serveraddr, "httptest");
	httpserver.start(2);
	loop.loop();
	
#ifdef _WIN32
	WSACleanup();
#endif
	return 0;
}