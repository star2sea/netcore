#ifndef __LOGGER__H
#define __LOGGER__H
#include "logstream.h"
#include "timestamp.h"
namespace netcore
{
	class Logger
	{
	public:
		enum LogLevel{
			TRACE,
			DEBUG,
			INFO,
			WARN,
			ERR,
			FATAL,
			NUM_LOG_LEVELS,
		};
		Logger(const char* file, int line, const char* level, const char* func);
		~Logger();

		Logger(const Logger &) = delete;
		Logger & operator=(const Logger&) = delete;

		LogStream & stream() {return stream_;}
    
		static LogLevel logLevel();
		static void setLogLevel(LogLevel level);
	private:
		void flush();
		static LogLevel logLevel_;
		LogStream stream_;
		const char * file_;
		const char * func_;
		int line_;
		const char * level_;
	};

	inline Logger::LogLevel Logger::logLevel() {return Logger::logLevel_;}
	inline void Logger::setLogLevel(LogLevel level) {Logger::logLevel_ = level;}

	#define LOG_TRACE \
		if (Logger::logLevel() <= Logger::TRACE) \
		Logger(__FILE__, __LINE__, "TRACE", __func__).stream()
	#define LOG_DEBUG \
		if (Logger::logLevel() <= Logger::DEBUG) \
		Logger(__FILE__, __LINE__, "DEBUG", __func__).stream()
	#define LOG_INFO \
		if (Logger::logLevel() <= Logger::INFO) \
		Logger(__FILE__, __LINE__, "INFO", __func__).stream()
	#define LOG_WARN Logger(__FILE__, __LINE__, "WARN", __func__).stream()
	#define LOG_ERROR Logger(__FILE__, __LINE__, "ERROR", __func__).stream()
	#define LOG_FATAL Logger(__FILE__, __LINE__, "FATAL", __func__).stream()

}

#endif
