#include "eventloopthread.h"
using namespace netcore;

EventLoopThread::EventLoopThread(EventLoop *loop, Acceptor * acceptor)
    :loop_(loop),
    acceptor_(acceptor),
    thread_(std::thread(std::bind(&EventLoopThread::threadFunc, this)))
{
    
}

EventLoopThread::~EventLoopThread()
{
    //todo
}
void EventLoopThread::join()
{
    //tood
}

void EventLoopThread::threadFunc()
{
    acceptor_->startAccept();
    loop_->loop();
}
