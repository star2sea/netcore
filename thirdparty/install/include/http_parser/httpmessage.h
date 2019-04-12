#ifndef __HTTP_MESSAGE_H
#define __HTTP_MESSAGE_H

#include "parser/http_parser.h"
#include <map>
#include <string>

namespace httpparser
{
	class HttpMessage
	{
	public:
		void setVersion(unsigned short major, unsigned short minor) { http_major_ = major; http_minor_ = minor; }
        void setKeepAlive(bool keepalive, bool addHeader = false) { keep_alive_ = keepalive; if (addHeader) setHeader("Connection", "keep-alive");}
        void setBody(const char *buf, size_t len, bool addHeader=false) { body_ = std::string(buf, len); if (addHeader) setHeader("Content-Length", len);}
        void setBody(const std::string & body, bool addHeader=false) { body_ = body; if (addHeader) setHeader("Content-Length", body.length());}
		void setHeader(const std::string &k, const std::string &v) { headers_[k] = v; }
        void setHeader(const std::string &k, int v) {headers_[k] = std::to_string(v);}
		void setStatus(unsigned int status_code) { status_code_ = status_code; }
		void setStatus(http_status status_code) { status_code_ = status_code; }
		void setStatusStr(const char *buf, size_t len) { status_str_ = std::string(buf, len); }
        void setStatusStr(const std::string & status_str) { status_str_ = status_str; }
		void setMethod(unsigned int method) { method_ = method; }
        void setMethod(http_method method) { method_ = method; }
        void setReqUrl(const std::string & url) { req_url_ = url; }
		void setReqUrl(const char *buf, size_t len) { req_url_ = std::string(buf, len); }

		std::string methodStr() const { return http_method_str(static_cast<http_method>(method_)); }
		std::string versionStr() const { char version[] = "HTTP/1.0"; version[5] = http_major_ + 48; version[7] = http_minor_ + 48; return version; }
		std::string statusStr() const { return http_status_str(static_cast<http_status>(status_code_)); }
        std::string headerStr() const;
		
		bool getKeepAlive() const { return keep_alive_; }
		std::string getBody() const { return body_; }
		std::string getReqUrl() const { return req_url_; }
		unsigned int getStatus() const { return status_code_; }
        std::string getHeader(std::string &k) const
        {
            auto iter = headers_.find(k);
            return iter != headers_.end() ? iter->second : "";
        }
        std::map<std::string, std::string> getHeaders() const {return headers_;}

        void reset();

	protected:
		unsigned short http_major_ = 1;
		unsigned short http_minor_ = 1;
		bool keep_alive_;
		std::map<std::string, std::string> headers_;
		std::string body_;

		unsigned int method_; // request only
		std::string req_url_; // request only

		unsigned int status_code_; // response only
		std::string status_str_;   // response only
	};

	class HttpRequest : public HttpMessage
	{
	public:
        enum http_parser_type msgtype_ = HTTP_REQUEST;
        std::string toStr() const;
	};

	class HttpResponse : public HttpMessage
	{
    public:
        enum http_parser_type msgtype_ = HTTP_RESPONSE;
        std::string toStr() const;
	};
	
}

#endif
