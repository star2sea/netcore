#include "httprequest.h"

bool HttpRequest::setMethod(const char *start, const char *end)
{
	std::string m(start, end);
	if (m == "GET")
	{
		method_ = kGet;
	}
	else if (m == "POST")
	{
		method_ = kPost;
	}
	else if (m == "HEAD")
	{
		method_ = kHead;
	}
	else if (m == "PUT")
	{
		method_ = kPut;
	}
	else if (m == "DELETE")
	{
		method_ = kDelete;
	}
	else
	{
		method_ = kInvalid;
	}
	return method_ != kInvalid;
}

const std::string HttpRequest::methodName() const
{
	switch (method_)
	{
	case kGet:
		return "GET";
		break;
	case kPost:
		return "POST";
		break;
	case kHead:
		return "HEAD";
		break;
	case kPut:
		return "PUT";
		break;

	case kDelete:
		return "DELETE";
		break;
	case kInvalid:
	default:
		return "Unknown";
		break;
	}
}

void HttpRequest::addHeader(const char *start, const char * colon, const char * end)
{
	std::string field(start, colon);
	++colon;
	while (colon < end && *colon == ' ')
	{
		++colon;
	}
	std::string value(colon, end);
	headers_[field] = value;
}

std::string HttpRequest::getHeader(const std::string & k) const
{
	std::string value;
	auto iter = headers_.find(k);
	if (iter != headers_.end())
	{
		value = iter->second;
	}
	return value;
}
