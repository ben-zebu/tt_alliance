#pragma once

#include <mutex>
#include <queue>

#include "Environment.h"

namespace abase {

    /// @class FileReader
    /// @brief Provides utility functions for filtering lines of text.
    ///
    /// The FileLineFilter class contains static methods for performing common text filtering operations,
    /// such as escaping special characters in a regex pattern, generating a regex pattern for a comment
    /// delimiter, and filtering a line of text based on a set of delimiters.
    class FileLineFilter {
        private:
            /// @brief Transforms a character into a regex string.
            /// @param ch a character
            /// @return a regex string
            static std::string char_regex(const char ch) {
                std::string rstr = "";
                if (ch == '\\' || ch == ']' || ch == '[' || ch == '^' || ch == '-') {
                    rstr += '\\'; 
                }
                return rstr + ch;
            }
            /// @brief Creates a regex pattern for a comment delimiter.
            /// @param delimiters Characters that delimit a comment.
            /// @return a string that represents the regex pattern for a comment delimiter.
            static std::string comment_regex(const std::string& delimiters);
            /// @brief Creates a regex pattern for a range of timesteps.
            /// @param delimiters Characters that delimit a range of timesteps.
            std::string range_regex(const std::string& delimiters);
        public:
            /// @brief Split a line of text into words.
            /// @param line The line of text to be filtered.
            /// @return A vector of strings, where each string represents a word in the line.
            static std::vector<std::string> filter(const std::string& line);
    };


    /// @class FileReader
    /// @brief Provides functionality for reading and navigating through a file line by line.
    ///
    /// The FileReader class is responsible for reading a file and providing methods to access the
    /// words in the file. 
    ///
    /// The class is designed to be thread-safe, with a mutex to ensure that multiple threads
    /// can safely access the file and buffer.
    class FileReader {
        private:
            /// @brief previous line of the file (only used for error messages)
            std::string prev_line;
            /// @brief current line of the file
            std::string line;
            /// @brief buffer of string for current line
            std::vector<std::string> buffer;
            /// @brief current position on the current line (which word)
            std::size_t word_rk = 0;
            /// @brief current line number (only used for error messages)
            std::size_t line_number = 0;
            
            /// @brief file name
            std::string filename;
            /// @brief input file stream
            std::ifstream input;
            /// @brief Mutex to ensure thread safety
            std::mutex mutex;

            /// @brief Update the buffer with a new line
            void buffer_update();

        public:
            /// @brief Constructor
            /// @param input_name name of the input file
            FileReader(const std::string& input_name);
            /// @brief return the current word in the buffer
            std::string get_word();
            /// @brief move forward from one word in the buffer
            void move();
            /// @brief move forward from one line in the buffer
            void move_line();
            /// @brief return the current line in the buffer
            std::string get_line() { return line; }
            /// @brief return a standard error message with the current line
            std::string context_error() const;
    };

}
