#ifndef __HTTP_CODEC_H
#define __HTTP_CODEC_H
#include "http_parser/httpparser.h"
#include "../codec.h"
#include "httpcontext.h"

namespace netcore
{
	class HttpCodec : public Codec
	{
	public:
		typedef std::function<void (const ConnectionPtr &, const httpparser::HttpMessage *, httpparser::HttpResponse *)> HttpCallback;
		HttpCodec(http_parser_type t);
		virtual ~HttpCodec() {}
		virtual void onMessage(const ConnectionPtr &conn, Buffer &buffer) override;
		void setHttpCallback(const HttpCallback &cb) { httpCallback_ = cb; }
	private:
		void defalutHttpCallback(const ConnectionPtr &, const httpparser::HttpMessage *, httpparser::HttpResponse *);
	private:
		HttpContext ctx_;
		HttpCallback httpCallback_;
	};
}

#endif