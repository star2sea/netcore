#ifndef __EVENTLOOP_THREAD_H
#define __EVENTLOOP_THREAD_H
#include "../utils/noncopyable.h"
#include "eventloop.h"
#include "acceptor.h"
#include "netaddr.h"
#include <thread>
#include "callback.h"
#include <memory>
namespace netcore
{
    class EventLoopThread :NonCopyable
    {
    public:
        EventLoopThread(const NetAddr & addr, const ConnectionCallback &conncb, const MessageCallback &msgcb);
        ~EventLoopThread();

		void setConnectionCallback(const ConnectionCallback & cb) { connectionCallback_ = cb; }
		void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }

		void join();

    private:
        void threadFunc(const NetAddr & addr);
    private:
        EventLoop * loop_;
        std::unique_ptr<Acceptor> acceptor_;
        std::thread thread_;
		std::mutex mtx_;
		std::condition_variable cond_;

		ConnectionCallback connectionCallback_;
		MessageCallback messageCallback_;
    };
}

#endif
