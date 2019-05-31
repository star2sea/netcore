#include "protobufcodec.h"
#include "../../../utils/logger.h"
#include "../../../net/connection.h"
#include "zlib.h" //adler32
using namespace netcore;

ProtobufCodec::ProtobufCodec()
{
	setErrorCallback(std::bind(&ProtobufCodec::defaultErrorCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
} 

void ProtobufCodec::onMessage(const ConnectionPtr &conn, Buffer &buf)
{
	while (buf.readAvailable() >= HeaderLen + MinMsgLen)
	{
		int32_t len = asInt32(buf.readBegin());
		if (len > MaxMsgLen || len < MinMsgLen)
		{
			protobufErrorCallback_(conn, buf, InvalidLength);
			break;
		}
		else if (buf.readAvailable() >= len + HeaderLen)
		{
			ErrorCode err = NoError;
			ProtoMsgPtr msg = parse(buf.readBegin() + HeaderLen, len, &err);
			if (err == NoError && msg)
			{
				buf.consume(HeaderLen + len);
				protobufMessageCallback_(conn, msg);
			}
			else
			{
				protobufErrorCallback_(conn, buf, err);
				break;
			}
		}
		else
		{
			break;
		}
	}
}

void ProtobufCodec::defaultErrorCallback(const ConnectionPtr &conn, Buffer &buf, ErrorCode err)
{
	LOG_ERROR << "ProtobufCodec onMessage Error: " << errorCodeToString(err);
	if (conn->isConnected())
	{
		conn->shutdown();
	}
}

google::protobuf::Message* ProtobufCodec::createMsgFromTypeName(const std::string &typeName)
{
	google::protobuf::Message *msg = NULL;
	const google::protobuf::Descriptor *descriptor =
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor)
	{
		const google::protobuf::Message *prototype = 
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			msg = prototype->New();
		}
	}
	return msg;
}

int32_t ProtobufCodec::asInt32(const char *buf)
{
	return (buf[0] << 24) + (buf[1] << 16) + (buf[2] << 8) + buf[3];
}

int16_t ProtobufCodec::asInt16(const char *buf)
{
	return (buf[0] << 8) + buf[1];
}

ProtoMsgPtr ProtobufCodec::parse(const char *buf, int len, ErrorCode * err)
{
	ProtoMsgPtr msg;
	int32_t expectedCheckSum = asInt32(buf + len - CheckSumLen);
	int32_t checksum = static_cast<int32_t> (::adler32(1, reinterpret_cast<const Bytef*>(buf), static_cast<int>(len - CheckSumLen)));
	if (expectedCheckSum == checksum)
	{
		int16_t namelen = asInt16(buf);
		if (namelen >= 2 && namelen <= len - TypeNameLen - CheckSumLen)
		{
			std::string typeName(buf + TypeNameLen, buf + TypeNameLen + namelen - 1);
			msg.reset(createMsgFromTypeName(typeName));
			if (msg)
			{
				const char *data = buf + TypeNameLen + namelen;
				int32_t datalen = len - TypeNameLen - namelen - CheckSumLen;
				if (msg->ParseFromArray(data, datalen))
				{
					*err = NoError;
				}
				else
				{
					*err = ParseError;
				}
			}
			else
			{
				*err = UnknownMsgType;
			}
		}
		else
		{
			*err = InvalidNameLen;
		}
	}
	else
	{
		*err = CheckSumError;
	}
	return msg;
}

void ProtobufCodec::fillEmptyBuffer(Buffer *buf, const ProtoMsgPtr &msg)
{
	assert(buf->readAvailable() == 0);
	const std::string &typeName = msg->GetTypeName();
	int32_t namelen = static_cast<int32_t>(typeName.size() + 1);

	const char namelen_buf[2] = {(namelen >> 8) & 0xff, namelen & 0xff };
	buf->append(namelen_buf, 2);
	buf->append(typeName.c_str(), namelen);

	int bytesize = msg->ByteSize();
	buf->ensureEnoughSpace(bytesize);
	uint8_t *start = reinterpret_cast<uint8_t*>(buf->writeBegin());
	uint8_t* end = msg->SerializeWithCachedSizesToArray(start);
	assert(end - start == bytesize);

	buf->hasWritten(bytesize);

	int32_t checksum = static_cast<int32_t>(::adler32(1, reinterpret_cast<const Bytef*>(buf->readBegin()), static_cast<int>(buf->readAvailable())));
	const char checksum_buf[4] = { (checksum >> 24) & 0xff, (checksum >> 16) & 0xff, (checksum >> 8) & 0xff, checksum & 0xff };
	buf->append(checksum_buf, 4);

	int32_t len = static_cast<int32_t>(buf->readAvailable());
	const char len_buf[4] = { (len >> 24) & 0xff, (len >> 16) & 0xff, (len >> 8) & 0xff, len & 0xff };
	buf->prepend(len_buf, 4);
}

const std::string& ProtobufCodec::errorCodeToString(ErrorCode errorCode)
{
	switch (errorCode)
	{
	case NoError:
		return NoErrorStr;
	case InvalidLength:
		return InvalidLengthStr;
	case CheckSumError:
		return CheckSumErrorStr;
	case InvalidNameLen:
		return InvalidNameLenStr;
	case UnknownMsgType:
		return UnknownMsgTypeStr;
	case ParseError:
		return ParseErrorStr;
	default:
		return UnknownErrorStr;
	}
}