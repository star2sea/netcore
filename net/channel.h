#ifndef __CHANNEL_H
#define __CHANNEL_H
#include "../utils/noncopyable.h"
#include "eventloop.h"

namespace netcore
{
	class EventLoop;

	class Channel :NonCopyable
	{
	public:
		enum
		{
			EVT_NONE = 0,
			EVT_R = 1,
			EVT_W = 2,
			EVT_RW = EVT_R | EVT_W,
		};

		typedef std::function<void()> ReadableCallback;
		typedef std::function<void()> WritableCallback;

		Channel(int fd, EventLoop * loop) :fd_(fd), loop_(loop), evt_(EVT_NONE), tied_(false) {}

		int fd() { return fd_; }

		void remove() { assert(isNonEvent()); loop_->removeChannel(this); }
		void reallocate(int fd);

		void enableReading() { evt_ |= EVT_R; update(); }
		void enableWriting() { evt_ |= EVT_W; update(); }
		void enableAll() { evt_ |= EVT_RW; update(); }

		void disableReading() { evt_ &= (~EVT_R); update(); }
		void disableWriting() { evt_ &= (~EVT_W); update(); }
		void disableAll() { evt_ = EVT_NONE; update(); }

		bool isReading() const { return evt_ & EVT_R; }
		bool isWriting() const { return evt_ & EVT_W; }
		bool isNonEvent() const { return !(evt_ & EVT_RW); }

		void handleReadable();
		void handleWritable();

		void handleReadableWithGuard() { if (readableCallback_) readableCallback_(); } 
		void handleWritableWithGuard() { if (writableCallback_) writableCallback_(); }

		void setReadableCallback(const ReadableCallback & cb) { readableCallback_ = cb; }
		void setWritableCallback(const WritableCallback & cb) { writableCallback_ = cb; }

		void tie(const std::shared_ptr<void>& obj) { tie_ = obj; tied_ = true; }

	private:
		void update() { loop_->updateChannel(this); }
	private:
		int fd_;
		EventLoop * loop_;
        int evt_;
		std::weak_ptr<void> tie_;
		bool tied_;
		ReadableCallback readableCallback_;
		WritableCallback writableCallback_; 
	};
}

#endif
