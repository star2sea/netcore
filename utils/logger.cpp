#include "logger.h"

using namespace netcore;

Logger::LogLevel Logger::logLevel_ = Logger::INFO;

Logger::Logger(const char* file, int line, const char * level, const char* func)
    : stream_(),
    file_(file),
    func_(func),
    line_(line),
    level_(level)
    
{   
    stream_ << TimeStamp::now() << " " << level_ << " ";
}

Logger::~Logger(){
    stream_ << " " << file_ << ':' << line_ << '\n';
    flush();
}

void Logger::flush(){
    LogStream::Buffer & buffer = stream_.buffer();
    fwrite(buffer.data(), 1, buffer.length(), stdout);
    fflush(stdout);
}