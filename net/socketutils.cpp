#include "socketutils.h"

namespace netcore
{
	namespace socketutils
	{
		inline void setNonBlocking(int fd)
		{
		#ifdef _WIN32
			u_long ul = 1;
			ioctlsocket(SOCKET_HANDLE(fd), FIONBIO, &ul);
		#else
			auto flags = fcntl(fd, F_GETFL, 0);
			flags |= O_NONBLOCK;
			fcntl(fd, F_SETFL, flags);
		#endif
		}

		int createSocket()
		{
			return SOCKET_FD(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
		}

		int createNonBlockingSocket()
		{
			int sockfd = createSocket();
			setNonBlocking(sockfd);
			return sockfd;
		}
	}

}
