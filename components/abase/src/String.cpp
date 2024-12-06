#include <algorithm>
#include <regex>

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

std::vector<std::string> str::split(const std::string& str) {
    std::string trimmed = str::trim(str);

    // Regex expression to split the line into words
    std::regex SplitRegex(R"((["'][^"']*["'])|(\S+))");
    auto wordsBegin = std::sregex_iterator(trimmed.begin(), trimmed.end(), SplitRegex);
    auto wordsEnd = std::sregex_iterator();

    // Process each word
    std::vector<std::string> result;
    for (auto it = wordsBegin; it != wordsEnd; ++it) {
        std::string word = it->str();
        // Remove leading and trailing quotes
        if (word.front() == '"' || word.front() == '\'') word.erase(0, 1);
        if (word.back() == '"' || word.back() == '\'') word.pop_back();

        if (!word.empty()) result.push_back(word);        
    }
    return result;
}

std::string str::replace(const std::string& str, const std::string& from, const std::string& to, std::size_t n) {
    std::string result = str;
    std::size_t count = 0;
    std::size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.length(), to);
        pos += to.length();
        count++;
        if (n > 0 && count >= n) break;
    }
    return result;
}