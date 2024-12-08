#pragma once

#include <fstream>
#include <mutex>
#include <string>

namespace abase {

    /// @class FileLogger
    /// @brief Class used to log messages in a file
    class FileLogger {
        private:
            /// @brief output file stream
            std::ofstream logFile_;
            /// @brief Mutex to ensure thread safety
            std::mutex mutex_;

        public:
            FileLogger() = default;
            /// @brief Constructor method
            /// @param logFilePath output file name 
            FileLogger(const std::string& logFilePath);
            /// @brief Destructor method
            virtual ~FileLogger() = default;
            /// @brief Method used to open the output file
            void open(const std::string& logFilePath);
            /// @brief Method used to write a message in the output file
            /// @param message 
            void write(const std::string& message);
    };

}