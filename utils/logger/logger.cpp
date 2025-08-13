#include "logger.h"
#include <filesystem>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "generalUtils.h"

namespace Utils {

    Logger* Utils::Logger::instance = nullptr;
    std::once_flag Utils::Logger::onceFlag;
    std::mutex Utils::Logger::logMutex;

    Logger::Logger() {
        try {
            std::filesystem::path currentPath = std::filesystem::current_path();
            if (!std::filesystem::exists(currentPath / "logs")) {
                std::filesystem::create_directory(currentPath / "logs");
            }
            else {
                if (!std::filesystem::is_empty(currentPath / "logs")) {
                    std::vector<std::filesystem::directory_entry> files;
                    for (const auto& entry : std::filesystem::directory_iterator(currentPath / "logs")) {
                        files.push_back(entry);
                    }
                    if (files.size() >= numberOfLogs) {
                        std::sort(files.begin(), files.end(),
                            [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
                                return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
                            });
                        std::filesystem::remove(files.front());
                    }
                }
            }
            auto logPath = currentPath / "logs" / (Utils::getCurrentDateTimeAsString() + ".txt");
            logFileStream = std::make_unique<std::ofstream>(logPath.string(), std::ios::out);
            if (logFileStream->is_open()) {
                logStream = logFileStream.get();
            }
            else {
                std::cerr << "Impossibile open log file, logging on std::cout" << std::endl;
                logStream = &std::cout;
            }
            *logStream << "********************" << Utils::getCurrentDateTimeAsString() << "********************" << std::endl;
        }
        catch (...) {
            std::cerr << "Error during initialization of log, logging on std::cout";
            logStream = &std::cout;
        }
    }

    Logger& Logger::log(logLevel_t logLevel, const char* invoker) {
        std::call_once(onceFlag, [] {
            Logger::instance = new Logger();
        });
        std::lock_guard<std::mutex>lock(logMutex);
        switch (logLevel) {
            case logLevel_t::INFO:
                *Logger::instance << "-INFO: ";
            break;
            case logLevel_t::DEBUG:
                *Logger::instance << "-DEBUG: ";
            break;
            case logLevel_t::WARNING:
                *Logger::instance << "-WARNING: ";
            break;
            case logLevel_t::CRITICAL:
                *Logger::instance << "-CRITICAL: ";
            break;
            default:
                /*nothing to do*/
            break;   
        }

        if (invoker != "") {
            *Logger::instance << getCurrentTimeAsString()
                << " [Thread " << std::this_thread::get_id() << "] From " << invoker << ": ";
        }
        else {
            *Logger::instance << getCurrentTimeAsString()
                << " [Thread " << std::this_thread::get_id() << "]: "; 
        }
        return *Logger::instance;
    }


}
