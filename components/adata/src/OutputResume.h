#pragma once

#include "Environment.h"
#include "FileLogger.h"

constexpr std::string_view RESUME_EXTENSION = ".res";
constexpr std::string_view LISTING_EXTENSION = ".lis";
constexpr std::string_view LOG_DIR_EXTENSION = ".log.d";

class OutputResume {
    private :
        /// @brief determine the base name of the input file
        std::string get_basename();

        /// @brief suppress previous results files
        void suppress_previous_results();

    protected:
        /// @brief output file stream
        abase::FileLogger logFile;
        /// @brief Mutex to ensure thread safety
        std::mutex mutex_;
        /// @brief input file name
        std::string input_filename;
    public:
        /// @brief Constructor method that suppress previous results and open the output file
        OutputResume(const std::string& filename);
        /// @brief Destructor method
        ~OutputResume();
        
        /// @brief Method used to write a message in the output file
        /// @param message 
        void write(const std::string& message) {
            std::lock_guard<std::mutex> lock(mutex_);
            logFile.write(message);
        }
        /// @brief Method that determine the name of the resume file based on the input file
        /// @return name of the output file
        std::string output_resume();

        /// @brief Method that determine the name of the listing file based on the input file
        /// @return name of the output file
        std::string output_listing();

        /// @brief Method that determine the name of the output folder based on the input file
        /// @return name of the output folder
        std::string output_folder();
};