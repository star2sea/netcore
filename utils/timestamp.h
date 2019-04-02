#ifndef __TIMESTAMP_H
#define __TIMESTAMP_H
#include <chrono>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

namespace netcore
{
	class TimeStamp
	{
	public:
		TimeStamp(std::chrono::time_point<std::chrono::system_clock> tp) :tp_(tp) {}
		
		static TimeStamp now() { return TimeStamp(std::chrono::system_clock::now()); }
		static TimeStamp afterNow(double sec) { return TimeStamp(std::chrono::system_clock::now() + std::chrono::microseconds(static_cast<long long>(sec * 1000000))); }

		friend double diffSeconds(TimeStamp &high, TimeStamp & low)
		{
			auto high_mills = std::chrono::duration_cast<std::chrono::milliseconds>(high.tp_.time_since_epoch()).count();
			auto low_mills = std::chrono::duration_cast<std::chrono::milliseconds>(low.tp_.time_since_epoch()).count();
			return static_cast<double>(high_mills - low_mills) / 1000;
		}
		friend bool operator<(TimeStamp &lhs, TimeStamp & rhs) { return lhs.tp_ < rhs.tp_; }
		friend bool operator==(TimeStamp & lhs, TimeStamp & rhs) { return lhs.tp_ == rhs.tp_; }
		friend std::ostream & operator<<(std::ostream & os, TimeStamp & ts)
		{
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(ts.tp_.time_since_epoch()).count() % 1000;
			auto t = std::chrono::system_clock::to_time_t(ts.tp_);
			return os << std::put_time(std::localtime(&t), "%Y%m%d %H:%M:%S.") << std::setfill('0') << std::setw(3) << ms;
		}
	
		const std::string toFormatString() const
		{
			auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp_.time_since_epoch()).count() % 1000;
			auto t = std::chrono::system_clock::to_time_t(tp_);
			char fmt[100];
			auto tm = std::localtime(&t);
			std::strftime(fmt, sizeof fmt, "%Y%m%d %H:%M:%S.", tm);
			return strcat(fmt, std::to_string(ms).c_str());
		}

	private:
		std::chrono::time_point<std::chrono::system_clock> tp_;
	};

	


}


#endif