#ifndef __PROTOBUF_CODEC_H
#define __PROTOBUF_CODEC_H
#include "../codec.h"
#include "../../buffer.h"
#include "../../callback.h"
#include <string>
#include <google/protobuf/message.h>

namespace netcore
{
	const std::string NoErrorStr = "NoError";
	const std::string InvalidLengthStr = "InvalidLength";
	const std::string CheckSumErrorStr = "CheckSumError";
	const std::string InvalidNameLenStr = "InvalidNameLen";
	const std::string UnknownMsgTypeStr = "UnknownMessageType";
	const std::string ParseErrorStr = "ParseError";
	const std::string UnknownErrorStr = "UnknownError";
}


namespace netcore
{
	typedef std::shared_ptr<google::protobuf::Message> ProtoMsgPtr;
	
	class ProtobufCodec : public Codec
	{
	public:
		enum ErrorCode
		{
			NoError = 0,
			InvalidLength,
			CheckSumError,
			InvalidNameLen,
			UnknownMsgType,
			ParseError,
		};
		typedef std::function<void(const ConnectionPtr &, const ProtoMsgPtr &)> ProtobufMessageCallback;
		typedef std::function<void(const ConnectionPtr &, Buffer &, ErrorCode)> ProtobufErrorCallback;

		ProtobufCodec();
		virtual ~ProtobufCodec() {}

		void setMessageCallback(const ProtobufMessageCallback & cb) { protobufMessageCallback_ = cb; }
		void setErrorCallback(const ProtobufErrorCallback & cb) { protobufErrorCallback_ = cb; }

		virtual void onMessage(const ConnectionPtr &conn, Buffer &buf) override;
		void fillEmptyBuffer(Buffer *buf, const ProtoMsgPtr &msg);

		static const std::string& errorCodeToString(ErrorCode errorCode);

	private:
		int32_t asInt32(const char *buf);
		int16_t asInt16(const char *buf);
		void defaultErrorCallback(const ConnectionPtr &, Buffer &, ErrorCode);
		google::protobuf::Message* createMsgFromTypeName(const std::string &typeName);
		ProtoMsgPtr parse(const char *buf, int len, ErrorCode * err);
	private:
		ProtobufMessageCallback protobufMessageCallback_;
		ProtobufErrorCallback protobufErrorCallback_;

		static const int HeaderLen = 4;
		static const int CheckSumLen = 4;
		static const int TypeNameLen = 2;
		static const int MinMsgLen = TypeNameLen + 2 + CheckSumLen; // typenamelen, typename, checksum 
		static const int MaxMsgLen = 64 * 1024 * 1024;
	};
}

#endif
