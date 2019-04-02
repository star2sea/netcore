#ifndef __LOGSTRAM_H
#define __LOGSTRAM_H
#include <iostream>
#include <string>
#include <string.h>
#include <cstring> //memset
#include "timestamp.h"

namespace netcore
{

	const int kSmallFixedBufSize = 4096;
	const int kLargeFixedBufSize = 4096 * 4096;


	template <int SIZE>
	class FixedBuffer{
	public:
		FixedBuffer(): cur_(buf_) {}

		FixedBuffer(const FixedBuffer &) = delete;
		FixedBuffer & operator=(const FixedBuffer &) = delete;

		void append(const char* buf, size_t len){
			if (static_cast<size_t>(avail()) > len){
				memcpy(cur_, buf, len);
				cur_ += len;
			}
		}

		const char* data() const {return buf_;}
		int length() const {return static_cast<int>(cur_ - buf_);}

		char * current() const {return cur_;}

		int avail() const {return static_cast<int>(end() - cur_);}

		void reset() {bzero(); cur_ = buf_;}

		void add(size_t len) {cur_ += len;}

	private:
		const char * end() const {return buf_ + sizeof(buf_);}
		void bzero() {memset(buf_, 0, sizeof buf_);}

		char buf_[SIZE] = {0};
		char * cur_;
	};

	class LogStream{
	public:
		typedef FixedBuffer<kSmallFixedBufSize> Buffer;
		LogStream() = default;
		LogStream(const LogStream &) = delete;
		LogStream& operator=(const LogStream &) = delete;

		LogStream & operator<< (char);
		LogStream & operator<< (unsigned char);
		LogStream & operator<< (const char *);
		LogStream & operator<< (const unsigned char *);
		LogStream & operator<< (const std::string &);
		LogStream & operator<< (short);
		LogStream & operator<< (int);
		LogStream & operator<< (long);
		LogStream & operator<< (long long);
		LogStream & operator<< (unsigned short);
		LogStream & operator<< (unsigned int);
		LogStream & operator<< (unsigned long);
		LogStream & operator<< (unsigned long long);
		LogStream & operator<< (float);
		LogStream & operator<< (double);
		LogStream & operator<< (TimeStamp & ts);

		Buffer & buffer() {return buffer_;}
		void resetBuffer() {buffer_.reset();}
	private:
		void append(const char *buf, size_t len) {buffer_.append(buf, len);}
		template <typename T> void formatInterger(T v);
		Buffer buffer_;
		static const int kMaxNumberSize = 32;
	};

}

#endif