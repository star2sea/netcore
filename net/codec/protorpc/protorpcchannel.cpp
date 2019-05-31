#include "protorpcchannel.h"
using namespace netcore;

void ProtoRpcChannel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
	::google::protobuf::RpcController* controller,
	const ::google::protobuf::Message* request,
	::google::protobuf::Message* response,
	::google::protobuf::Closure* done)
{

}

void ProtoRpcChannel::onRpcMessage(const ConnectionPtr &conn, const ProtoMsgPtr &msg)
{
	RpcMsgPtr rpcmsg = std::dynamic_pointer_cast<RpcMessage>(msg);
}
