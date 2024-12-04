#include <algorithm>

#include "Environment.h"
#include "String.h"

using namespace abase;

std::string str::trim(const std::string& str) {
    auto start = str.find_first_not_of(STRING_DELIMITER);
    if (start == std::string::npos) {
        return "";
    }
    auto end = str.find_last_not_of(STRING_DELIMITER);
    return str.substr(start, end - start + 1);
}

std::string str::lowercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string str::uppercase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}