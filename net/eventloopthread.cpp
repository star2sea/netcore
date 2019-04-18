#include "eventloopthread.h"
using namespace netcore;

EventLoopThread::EventLoopThread(const NetAddr & addr, const ConnectionCallback &conncb, const MessageCallback &msgcb)
	:loop_(NULL),
	thread_(std::thread(std::bind(&EventLoopThread::threadFunc, this, addr))),
	connectionCallback_(conncb),
	messageCallback_(msgcb)
{

}

EventLoopThread::~EventLoopThread()
{
	join();
}

void EventLoopThread::join()
{
	{
		std::unique_lock<std::mutex> lock(mtx_);
		cond_.wait(lock, [&] {return (loop_ != NULL) && acceptor_; });
		acceptor_->stopAccept();
		loop_->quit();
	}
		
	if (thread_.joinable())
		thread_.join();
}

void EventLoopThread::threadFunc(const NetAddr & addr)
{
	EventLoop loop;

	{
		std::unique_lock<std::mutex> lock(mtx_);
		
		acceptor_ = std::unique_ptr<Acceptor>(new Acceptor(&loop, addr));
		acceptor_->setConnectionCallback(connectionCallback_);
		acceptor_->setMessageCallback(messageCallback_);
		acceptor_->startAccept();

		loop_ = &loop;
		cond_.notify_one();
	}
	
    loop.loop();
}
