#pragma once

#include <string>
#include <vector>

#define STRING_DELIMITER " \t\r\n"
#define COMMENT_DELIMITER "#;"
#define QUOTATION_DELIMITER "'\""

/// @brief String manipulation utilities
/// @namespace str
/// This namespace provides a collection of string manipulation functions
/// for common operations such as trimming whitespace, and case conversion.
namespace str {

    /// @brief value used to represent an unset precision for double conversion to string
    const double UNSET_STRING_PRECISION = 20;

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
    /// @brief Split a string into a vector of strings
    /// @param str the string to split
    /// @param delimiter the character to split the string on
    /// @return a vector of strings
    std::vector<std::string> split(const std::string& str);
    /// @brief Replace occurrences of a substring in a string
    /// @param str the string to search
    /// @param from the substring to search for
    /// @param to the substring to replace with
    /// @param n the maximum number of replacements to make (0 for all)
    /// @return the modified string
    std::string replace(const std::string& str, const std::string& from, const std::string& to, std::size_t n = 0);

    /// @brief Convert a double to a string with a specified precision
    /// @param value double value to convert
    /// @param precision precision of the conversion
    /// @return string representation of the double
    std::string to_string(const double value, const std::size_t precision = UNSET_STRING_PRECISION);

    /// @brief Convert a double to a string with a specified precision in scientific format
    /// @param value double value to convert
    /// @param precision precision of the conversion
    /// @return string representation of the double
    std::string to_string_exp(const double value, const std::size_t precision = UNSET_STRING_PRECISION);

}