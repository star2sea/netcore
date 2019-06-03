#include "protorpcchannel.h"
#include "../../../utils/logger.h"
using namespace netcore;

void ProtoRpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
	::google::protobuf::RpcController* controller,
	const ::google::protobuf::Message* request,
	::google::protobuf::Message* response,
	::google::protobuf::Closure* done)
{
	RpcMessage rpcmsg;
	rpcmsg.set_service(method->service()->full_name());
	rpcmsg.set_method(method->name());
	rpcmsg.set_message(request->SerializeAsString());

	Buffer buf;
	ProtobufCodec *codec = static_cast<ProtobufCodec*>(conn_->getConnectionCodec());
	codec->fillEmptyBuffer(&buf, &rpcmsg);
	conn_->send(buf);
}

void ProtoRpcChannel::onRpcMessage(const ConnectionPtr &conn, const ProtoMsgPtr &msg)
{
	ErrorCode error = WRONG_PROTO;
	RpcMsgPtr rpcmsg = std::dynamic_pointer_cast<RpcMessage>(msg);
	auto iter = services_.find(rpcmsg->service());
	if (iter != services_.end())
	{
		google::protobuf::Service* service = iter->second;
		const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
		const google::protobuf::MethodDescriptor* method = desc->FindMethodByName(rpcmsg->method());
		if (method)
		{
			std::unique_ptr<google::protobuf::Message> request(service->GetRequestPrototype(method).New());
			if (request->ParseFromString(rpcmsg->message()))
			{
				service->CallMethod(method, NULL, request.get(), NULL, NULL);
				error = NO_ERR;
			}
			else
			{
				error = INVALID_REQUEST;
			}
		}
		else
		{
			error = NO_METHOD;
		}
	}
	else
	{
		error = NO_SERVICE;
	}

	if (error != NO_ERR)
	{
		LOG_ERROR << "ProtoRpcChannel::onRpcMessage error, errno = " << error;
		if (conn->isConnected())
		{
			conn->shutdown();
		}
	}
}
