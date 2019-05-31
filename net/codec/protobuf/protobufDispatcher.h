#ifndef __PROTOBUF_DISPATCH_H
#define __PROTOBUF_DISPATCH_H
#include "protobufCodec.h"
#include "../../connection.h"
namespace netcore
{
	class Callback
	{
	public:
		virtual ~Callback() = default;
		virtual void onMessage(const ConnectionPtr &, const ProtoMsgPtr &) = 0;
	};

	template <class T>
	class CallbackT : public Callback
	{
	public:
		typedef std::function<void(const ConnectionPtr &, const std::shared_ptr<T> &)> ProtobufMessageCallback;

		CallbackT(const ProtobufMessageCallback & cb): callback_(cb) {}

		virtual void onMessage(const ConnectionPtr &conn, const ProtoMsgPtr &msgPtr)
		{
			std::shared_ptr<T> realmsg = std::dynamic_pointer_cast<T>(msgPtr);
			callback_(conn, realmsg);
		}
	private:
		ProtobufMessageCallback callback_;
	};
	
	
	class ProtobufDispatcher
	{
	public:
		typedef std::function<void(const ConnectionPtr &, const ProtoMsgPtr &)> ProtobufMessageCallback;

		ProtobufDispatcher(const ProtobufMessageCallback &cb) : defaultCallback_(cb) {}

		~ProtobufDispatcher() 
		{
			for (auto iter = callbacks_.begin(); iter != callbacks_.end(); ++iter)
			{
				delete iter->second;
			}
		}
		
		template <class T>
		void registerMessageCallback(const typename CallbackT<T>::ProtobufMessageCallback& cb)
		{
			Callback *cbT = static_cast<Callback*>(new CallbackT<T>(cb));
			callbacks_[T::descriptor()] = cbT;
		}

		void onMessage(const ConnectionPtr &conn, const ProtoMsgPtr &msgPtr)
		{
			auto iter = callbacks_.find(msgPtr->GetDescriptor());
			if (iter != callbacks_.end())
			{
				iter->second->onMessage(conn, msgPtr);
			}
			else
			{
				defaultCallback_(conn, msgPtr);
			}
		}


	private:
		typedef std::map<const google::protobuf::Descriptor*, Callback*> CallbackMap;
		CallbackMap callbacks_;
		ProtobufMessageCallback defaultCallback_;
	};
}


#endif