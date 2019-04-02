#include  "logstream.h"
#include <algorithm>
namespace netcore
{

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;

template <typename T>
size_t convert(char buf[], T v)
{
    T i = v;
    char *p = buf;
    do{
        auto t = static_cast<int>(i % 10);
        *p++ = zero[t];
        i /= 10;
    }while (i != 0);
    if (v < 0)
        *p++ = '-';
    *p = '\0';
    std::reverse(buf, p);
    return p-buf;
}

template <typename T>
void LogStream::formatInterger(T v){
    if (buffer_.avail() > kMaxNumberSize){
        auto len = convert<T> (buffer_.current(), v);
        buffer_.add(len);
    }
}

LogStream & LogStream::operator<< (char c){
    buffer_.append(&c, 1);
    return *this;
}
LogStream & LogStream::operator<< (unsigned char c){
    char t = static_cast<char> (c);
    return operator<<(t);
}

LogStream & LogStream::operator<< (const char *buf){
    if (buf){
        buffer_.append(buf, strlen(buf));
    }
    else{
        buffer_.append("(null)", 6);
    }
    return *this;
   
}
LogStream & LogStream::operator<< (const unsigned char *buf){
    return operator<<(reinterpret_cast<const char *> (buf));
}
LogStream & LogStream::operator<< (const std::string &str){
    buffer_.append(str.c_str(), str.size());
    return *this;
}
LogStream & LogStream::operator<< (short v){
    return operator<<(static_cast<int>(v));
}
LogStream & LogStream::operator<< (int v){
    formatInterger(v);
    return *this;
}
LogStream & LogStream::operator<< (long v){
    formatInterger(v);
    return *this;
}
LogStream & LogStream::operator<< (long long v){
    formatInterger(v);
    return *this;
}
LogStream & LogStream::operator<< (unsigned short v){
    return operator<<(static_cast<unsigned int>(v));
}
LogStream & LogStream::operator<< (unsigned int v){
    formatInterger(v);
    return *this;
}   
LogStream & LogStream::operator<< (unsigned long v){
    formatInterger(v);
    return *this;
}
LogStream & LogStream::operator<< (unsigned long long v){
    formatInterger(v);
    return *this;
}
LogStream & LogStream::operator<< (float v){
    return operator<<(static_cast<double>(v));
}
LogStream & LogStream::operator<< (double v){
    if (buffer_.avail() > kMaxNumberSize){
        auto len = snprintf(buffer_.current(), kMaxNumberSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;

}

LogStream & LogStream::operator<< (TimeStamp & ts) {
	return operator<<(ts.toFormatString());
}

}