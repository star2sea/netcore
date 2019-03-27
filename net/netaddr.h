#ifndef __NETADDR_H
#define __NETADDR_H
#include <string>

namespace netcore
{
	class NetAddr
	{
	public:
		NetAddr() = default;
		NetAddr(const struct sockaddr_in & addr) : addr_(addr) {}
		NetAddr(const char *ip, short port);
		NetAddr(short port);

		std::string toIpPort() const;

		struct sockaddr * toSockAddr() const;

	private:
		struct sockaddr_in addr_;
	};
}

#endif