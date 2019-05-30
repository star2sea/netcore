#ifndef __CODEC_H
#define __CODEC_H
#include "../callback.h"
#include "../buffer.h"
namespace netcore
{
	class Codec
	{
	public:
		virtual void onMessage(const ConnectionPtr &conn, Buffer &buffer) = 0;
		virtual ~Codec() {}
	};
}


#endif
