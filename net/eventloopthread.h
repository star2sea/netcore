#ifndef __EVENTLOOP_THREAD_H
#define __EVENTLOOP_THREAD_H
#include "../utils/noncopyable.h"
#include "eventloop.h"
#include "acceptor.h"
#include <thread>
namespace netcore
{
    class EventLoopThread:NonCopyable
    {
    public:
        EventLoopThread(EventLoop *loop, Acceptor * acceptor);
        ~EventLoopThread();
        void join();
    private:
        void threadFunc();
    private:
        EventLoop * loop_;
        Acceptor * acceptor_;
        std::thread thread_;
    };
}

#endif
