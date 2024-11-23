#include <iostream>

#include "TranslationManager.h"

#include "ErrorManager.h"

using namespace abase;

void ErrorManager::logMessage(const std::string& type, const std::string& message) {
    std::string msg = "[" + translate(type) + "] " + message;
    if (logger_) logger_->write(msg);
    std::cerr << msg << std::endl;
}


// Initialize the error manager with a logger
void ErrorManager::init(std::shared_ptr<FileLogger> logger) {
    logger_ = logger;
}

// Log an error message and terminate the program
void ErrorManager::logError(const std::string& message) {
    logMessage("ERROR", message);
    std::terminate();
}

// Log an input error message and terminate the program
void ErrorManager::logInputError(const std::string& message) {
    logMessage("INPUT_ERROR", message);
    std::terminate();
}

// Log a warning message
void ErrorManager::logWarning(const std::string& message) {
    logMessage("WARNING", message);
}
