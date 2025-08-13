#include "generalUtils.h"


namespace Utils {

	std::string getCurrentDateTimeAsString() {
		const auto now = std::chrono::system_clock::now();
		const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

		std::tm tm{};
#ifdef _WIN32
		localtime_s(&tm, &t_c);    
#else
		localtime_r(&t_c, &tm);    
#endif
		std::ostringstream oss;
		oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
		return oss.str();
	}

	std::string getCurrentTimeAsString() {
		const auto now = std::chrono::system_clock::now();
		const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

		std::tm tm{};
#ifdef _WIN32
		localtime_s(&tm, &t_c);
#else
		localtime_r(&t_c, &tm);
#endif

		std::ostringstream oss;
		oss << std::put_time(&tm, "%H:%M:%S");
		return oss.str();
	}

}