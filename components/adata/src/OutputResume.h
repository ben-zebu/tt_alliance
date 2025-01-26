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
        /// @brief create the results folder
        void create_results_folder();

    protected:
        /// @brief output file stream
        abase::FileLogger logFile;
        /// @brief Mutex to ensure thread safety
        std::mutex mutex_;
        /// @brief input file name
        std::string input_filename;

        /// @brief number of columns in the output file
        std::size_t columns = 80;

    public:
        /// @brief Constructor method
        OutputResume() = default;
        /// @brief Destructor method
        virtual ~OutputResume() = default;

        /// @brief Method used to initialize the output file. The name of the output file is based on the input file
        /// @param filename name of the input file 
        void init(const std::string& filename);

        /// @brief Method that determine the name of the resume file based on the input file
        /// @return name of the output file
        std::string output_resume();
        /// @brief Method that determine the name of the listing file based on the input file
        /// @return name of the output file
        std::string output_listing();
        /// @brief Method that determine the name of the output folder based on the input file
        /// @return name of the output folder
        std::string output_folder();

        /// @brief Method used to write a message in the output file
        /// @param message string to write in the output file
        void write(const std::string& message);
        /// @brief Method used to print the header in the main output file
        void write_header();
};