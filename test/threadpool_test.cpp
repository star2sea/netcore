#include "../utils/threadpool.h"
#include "../utils/logger.h"
#ifdef _WIN32
#include <Windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) sleep(ms/1000.0)
#endif

using namespace netcore;

void test(int i)
{
	LOG_INFO << i * i;
}

int main(int argc, char *argv[])
{
	ThreadPool threadpool(2);
	threadpool.start();
	for (auto i = 0; i < 20; ++i)
	{
		threadpool.run(std::bind(test, i));
	}
	SLEEP_MS(1000);
	threadpool.stop();
	return 0;
}