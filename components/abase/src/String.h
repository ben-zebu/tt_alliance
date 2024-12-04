#pragma once

#include <string>

#define STRING_DELIMITER " \t\r\n"
#define COMMENT_DELIMITER "#;"
#define QUOTATION_DELIMITER "'\""

namespace str {

    /// @brief Remove leading and trailing spaces from a string
    /// @param str the string to trim
    /// @return the trimmed string
    std::string trim(const std::string& str);
    /// @brief Return a lowercase version of a string
    /// @param str the original string
    /// @return the lowercase string
    std::string lowercase(const std::string& str);
    /// @brief Return a uppercase version of a string
    /// @param str the original string
    /// @return the uppercase string
    std::string uppercase(const std::string& str);

}