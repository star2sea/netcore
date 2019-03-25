#include "../utils/timestamp.h"
using namespace netcore;

int main()
{
	std::cout << TimeStamp::now() << std::endl;
	std::cout << TimeStamp::afterNow(0.05) << std::endl;
	std::cout << (TimeStamp::now() == TimeStamp::afterNow(0.000000005)) << std::endl;

	auto t1 = TimeStamp::now();
	auto t2 = t1;

	std::cout << (t1 == t2) << std::endl;
}