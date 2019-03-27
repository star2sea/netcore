#include "netaddr.h"
#include "socket.h"
#include <iostream>
#include <cstring> //memset

using namespace netcore;

NetAddr::NetAddr(const char *ip, short port)
{
	std::memset(&addr_, 0, sizeof addr_);
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	if (inet_pton(AF_INET, ip, &addr_.sin_addr.s_addr) < 0)
	{
		std::cout << "InetAddr::InetAddr error, invalid ip address" << std::endl;
	}
}

NetAddr::NetAddr(short port)
{
	std::memset(&addr_, 0, sizeof addr_);
	addr_.sin_family = AF_INET;
	addr_.sin_port = htons(port);
	addr_.sin_addr.s_addr = htonl(INADDR_ANY);
}

std::string NetAddr::toIpPort() const
{
	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr_.sin_addr.s_addr, ip, sizeof ip);
	short port = ntohs(addr_.sin_port);
	char buf[INET_ADDRSTRLEN + 3];
	snprintf(buf, sizeof buf, "%s:%d", ip, port);
	return buf;
}

struct sockaddr * NetAddr::toSockAddr() const
{
	return (struct sockaddr *)&addr_;
}
