#ifndef __BUFFER_H
#define __BUFFER_H
#include <vector>
#include <string>
namespace netcore
{
	class Buffer
	{
	public:
		Buffer(size_t t) : vec_(t), readIndex_(0), writeIndex_(0) {}
		
		char * writeBegin() { return begin() + writeIndex_; }
		const char *writeBegin() const { return begin() + writeIndex_; }
		const char * readBegin() const { return begin() + readIndex_; }

		std::string getAllAsString();
		void consume(size_t n);
		void consumeUntil(const char *);
		void consumeAll() { readIndex_ = 0; writeIndex_ = 0; }
 		void hasWritten(size_t n);
		void append(const char *buf, size_t n);
		void append(const std::string &str);

		size_t readAvailable() { return writeIndex_ - readIndex_; }
		size_t writeAvailable() { return vec_.size() - writeIndex_; }

		const char * findCRLF() const;

	private:
		char * begin() { return &*vec_.begin(); }
		const char *begin() const {return &*vec_.begin(); }
		void ensureEnoughSpace(size_t n);

	private:
		size_t readIndex_;
		size_t writeIndex_;
		std::vector<char> vec_;
		static const char CRLF[];
	};
}

#endif