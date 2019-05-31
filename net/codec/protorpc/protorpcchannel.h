#ifndef __PROTORPC_CHANNEL_H
#define __PROTORPC_CHANNEL_H
#include "google/protobuf/service.h"
#include "../../connection.h"
#include "../../callback.h"
#include "../protobuf/protobufcodec.h"
#include "rpcmessage.pb.h"

namespace netcore
{
	class ProtoRpcChannel : public google::protobuf::RpcChannel
	{
	public:
		typedef std::shared_ptr<RpcMessage> RpcMsgPtr;
		explicit ProtoRpcChannel(const ConnectionPtr & conn) : conn_(conn) {}
		~ProtoRpcChannel() override {};

		void registerRpcService(google::protobuf::Service * service) 
		{
			const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
			services_[desc->full_name()] = service;
		}

		void CallMethod(const ::google::protobuf::MethodDescriptor* method,
			::google::protobuf::RpcController* controller,
			const ::google::protobuf::Message* request,
			::google::protobuf::Message* response,
			::google::protobuf::Closure* done) override;

		void onRpcMessage(const ConnectionPtr &conn, const ProtoMsgPtr &msg);

	private:
		typedef std::map<std::string, google::protobuf::Service *> ServiceMap;
		ServiceMap services_;
		ConnectionPtr conn_;

	};
}

#endif
