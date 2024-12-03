#pragma once

#include <memory>

#include "FileLogger.h"

namespace abase {


// ErrorManager Class Definition
class ErrorManager {
    private:
        /// @brief Private constructor for Singleton Pattern
        ErrorManager() {};
        /// @brief Shared pointer to a `FileLogger`
        std::shared_ptr<FileLogger> logger_;

        /// @brief Helper function to log messages
        /// @param type type of message (error, input_error or warning)
        /// @param message output message 
        /// @warning No output message translation are done in this function.
        void logMessage(const std::string& type, const std::string& message);

    public:
        /// @brief Get the instance of the ErrorManager (Singleton Pattern)
        static ErrorManager& getInstance() {
            static ErrorManager instance;
            return instance;
        }

        /// @brief Delete copy constructor
        ErrorManager(const ErrorManager&) = delete;
        /// @brief Delete assignment operator
        ErrorManager& operator=(const ErrorManager&) = delete;

        /// @brief Initialize the error manager with a logger
        /// @param logger shared pointer to a `FileLogger`
        void init(std::shared_ptr<FileLogger> logger);

        /// @brief Log an error message and terminate the program
        /// @param message output message
        void logError(const std::string& message);
        /// @brief Log an input error message and terminate the program
        /// @param message output message
        void logInputError(const std::string& message);
        /// @brief Log an input error message and terminate the program
        /// @param message output message
        /// @param filecontext context of the file
        void logFileInputError(const std::string& message, const std::string& filecontext);        
        /// @brief Log an warning message and terminate the program
        /// @param message output message
        void logWarning(const std::string& message);
    };

}

/// @brief Alias function for error output
/// @param message output message
inline void error(const std::string message) {
    abase::ErrorManager::getInstance().logError(message);
}

/// @brief Alias function for input error output
/// @param message output message
inline void input_error(const std::string message) {
    abase::ErrorManager::getInstance().logInputError(message);
}

/// @brief Alias function for file input error output
/// @param message output message
inline void file_input_error(const std::string message, const std::string filecontext) {
    abase::ErrorManager::getInstance().logFileInputError(message, filecontext);
}


/// @brief Alias function for warning output
/// @param message output message
inline void warning(const std::string message) {
    abase::ErrorManager::getInstance().logWarning(message);
}