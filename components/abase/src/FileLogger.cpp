#include <iostream>
#include <stdexcept>

#include "FileLogger.h"

using namespace abase;

FileLogger::FileLogger(const std::string& logFilePath) {
    logFile_.open(logFilePath, std::ios::out | std::ios::app);
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

