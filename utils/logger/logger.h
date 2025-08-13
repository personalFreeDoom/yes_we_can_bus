/**************************************************************
* Logger is part of project xBus, for correctly get
* date/time need of files "generalUtils.h and generalUtils.cpp
* Use this code like you want.
* Author: Vincenzo Ziccardi
* From: Italy
* @mail: vincenzoziccardi19@gmail.com
* 01/07/2025
* "Feel as free as your code should be."
* ************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <ranges>

#define LOGINVOKER __func__ 

namespace Utils {

	class Logger final {
	public:
		/*typedefs*/
		using maxLog_t = const unsigned char;
		enum class logLevel_t {
			INFO,
			DEBUG,
			WARNING,
			CRITICAL
		};

	public:
		/*instance getter (singleton pattern)*/
		/*****************************************
		* Write on log just using the "<<" operator
		* example: log() << "StringToLog",
		* if you want kwnow who is invoking the logger
		* just add INVOKER like argument so:
		* log(INVOKER) << "StringToLog"
		* ***************************************/
		static Logger& log(logLevel_t logLevel = logLevel_t::INFO, const char* invoker = "");


	public:
		/*operators overload*/

		/*Deleted operators (singleton pattern)*/
		Logger& operator = (const Logger&) = delete;
		Logger& operator = (Logger&&) = delete;

		/*Overload << for C++ manipulators (example std::endl)*/
		Logger& operator << (std::ostream& (*manip)(std::ostream&)) {
			*logStream << manip;
			return *this;
		}
		/*Overload << for C++ generic types*/
		template <typename T>
		Logger& operator << (const T& message) {
			*logStream << message;
			return *this;
		}
		//Overload << for containers
		/*Overload << for C++ std::vector*/
		template<typename T>
		Logger& operator << (const std::vector<T>& vector) {
			std::ostringstream stream;
			for (auto vectorElement : vector) {
				stream << vectorElement << " ";
			}
			*logStream << stream.str();
			return *this;
		}
		/*Overload << for C++ std::map*/
		template<typename T, typename V>
		Logger& operator << (const std::map<T, V>& map) {
			std::ostringstream stream;
			for (auto mapElement : map) {
				stream << mapElement.first << ":" << mapElement.second << " ";
			}
			*logStream << stream.str();
			return *this;
		}

	private:
		/*constructor, private for singleton*/
		explicit Logger();
		/*destructor, not virtual (final class), default*/
		~Logger() = default;
		/*copy constructors, deleted singleton pattern*/
		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		/*static private instance for singleton pattern*/
		static Logger* instance;
		/*ofstream file pointer*/
		std::unique_ptr<std::ofstream> logFileStream = nullptr;
		/*ostream pointer: points to logFileStream if opened correctly, otherwise to std::cout*/
		std::ostream* logStream = nullptr;
		/*mutex for multithreading logging*/
		static std::mutex logMutex;
		/*once_flag to ensure a single Logger instance (thread-safe singleton)*/
		static std::once_flag onceFlag;
		/*Maximum number of log files; oldest files will be deleted if the number exceeds numberOfLogs*/
		maxLog_t numberOfLogs = 10;

	};

};

#endif

