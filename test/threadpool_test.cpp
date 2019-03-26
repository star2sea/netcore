#include "../utils/threadpool.h"

using namespace netcore;

void test(int i)
{
	std::cout << i * i << std::endl;
}

int main()
{
	ThreadPool threadpool(4);
	threadpool.start();
	for (auto i = 0; i < 20; ++i)
	{
		threadpool.run(std::bind(test, i));
	}
	for (auto i = 0; i < 1000; ++i)
		for (auto j = 0; j < 1000; ++j);

	threadpool.stop();
	return 0;
}