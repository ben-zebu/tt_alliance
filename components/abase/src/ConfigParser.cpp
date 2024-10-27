#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "TranslationManager.h"
#include "ConfigParser.h"

#define STRING_DELIMITER " \t\r\n"
#define COMMENT_DELIMITER "#;"
#define QUOTATION_DELIMITER "'\""

// Initialize global instance
ConfigParser globalConfigParser;

//
// Private methods
//

std::string ConfigParser::_findValue_(const std::string& key) const {
    auto it = config_map.find(key);
    if (it == config_map.end()) {
        throw std::runtime_error("Unknown key: " + key);
    }
    return it->second;
}

std::string ConfigParser::trim(const std::string& str) const {
    // Trim leading and trailing string delimiters 
    auto start = str.find_first_not_of(STRING_DELIMITER);
    if (start == std::string::npos) {
        return "";
    }
    auto end = str.find_last_not_of(STRING_DELIMITER);
    std::string trimed_str = str.substr(start, end - start + 1);

    // Suppress quotation marks
    start = trimed_str.find_first_of(QUOTATION_DELIMITER);
    end = trimed_str.find_last_of(QUOTATION_DELIMITER);

    if (start == 0 && end == trimed_str.size() - 1) {
        trimed_str = trimed_str.substr(start + 1, end - start - 1);
    }

    return trimed_str;
}

template<>
int ConfigParser::_convertValue_<int>(const std::string& value) const {
    try {
        return std::stoi(value);
    } catch (const std::exception& e) {
        throw std::runtime_error(translate("CP_INTEGER_CONVERT", value));
    }
}

template<>
float ConfigParser::_convertValue_<float>(const std::string& value) const {
    try {
        return std::stof(value);
    } catch (const std::exception& e) {
        throw std::runtime_error(translate("CP_FLOAT_CONVERT", value));
    }
}

template<>
std::string ConfigParser::_convertValue_<std::string>(const std::string& value) const {
    return value;
}

//
// Public methods
//

void ConfigParser::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open configuration file: " + filename);
    }

    std::string line, key, value;
    while (std::getline(file, line)) {
        auto comment = line.find_first_of(COMMENT_DELIMITER);
        if (comment != std::string::npos) {
            line = line.substr(0, comment);
        }

        if (line.empty()) {
            continue;
        }

        auto pos = line.find('=');
        if (pos != std::string::npos) {
            key = trim(line.substr(0, pos));
            value = trim(line.substr(pos + 1));
            if (!key.empty() && !value.empty()) {
                config_map[key] = value;
            }
        }
    }

    file.close();
}

void ConfigParser::setValue(const std::string& key, const std::string& value) {
    config_map[key] = value;
}

template<typename T>
T ConfigParser::getValue(const std::string& key) const {
    return _convertValue_<T>(_findValue_(key));
}

template float ConfigParser::getValue<float>(const std::string&) const;
template int ConfigParser::getValue<int>(const std::string&) const;
template std::string ConfigParser::getValue<std::string>(const std::string&) const;

void ConfigParser::printConfig() const {
    for (const auto& [key, value] : config_map) {
        std::cout << key << " = " << value << std::endl;
    }
}