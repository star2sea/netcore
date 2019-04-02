#include "../utils/logger.h"
#include "../utils/logstream.h"
using namespace netcore;
int main(){
    LOG_TRACE << "hello world " << -123;
    Logger::setLogLevel(Logger::INFO);
    LOG_TRACE << "hello world again" << -123 << 1.2 << 1.4444232323;
    LOG_INFO << "hello world again" << " " << -123 << " " << 1.2 << " " << -1.4444232323;
    return 0;
}