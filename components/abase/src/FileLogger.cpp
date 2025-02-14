#include <iostream>

#include "Environment.h"
#include "FileLogger.h"

using namespace abase;

FileLogger::FileLogger(const std::string& logFilePath) {
    open(logFilePath);
}

void FileLogger::open(const std::string& logFilePath) {
    logFile_.open(logFilePath, std::ios::out);
    if (!logFile_.is_open()) {
        throw std::runtime_error("Failed to open log file: " + logFilePath + " !");
    }

}

void FileLogger::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);  // Ensure thread safety
    if (logFile_.is_open()) {
        logFile_ << message << std::endl;
    }
}

