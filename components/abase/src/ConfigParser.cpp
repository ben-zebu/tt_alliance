#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "Environment.h"
#include "ErrorManager.h"
#include "TranslationManager.h"
#include "ConfigParser.h"

using namespace abase;

// Initialize global instance
namespace abase {
    ConfigParser globalConfigParser;
}

//
// Private methods
//

std::string ConfigParser::_findValue_(const std::string& key) const {
    auto it = config_map.find(key);
    if (it == config_map.end()) {
        error(translate("UNKNOW_CONFIG_KEY", key));
    }
    return it->second;
}

std::string ConfigParser::trim(const std::string& str) const {
    // Trim leading and trailing string delimiters 
    std::string trimed_str = str::trim(str);

    // Suppress quotation marks
    std::size_t start = trimed_str.find_first_of(QUOTATION_DELIMITER);
    std::size_t end = trimed_str.find_last_of(QUOTATION_DELIMITER);

    if (start == 0 && end == trimed_str.size() - 1) {
        trimed_str = trimed_str.substr(start + 1, end - start - 1);
    }

    return trimed_str;
}

template<>
std::size_t ConfigParser::_convertValue_<std::size_t>(const std::string& value) const {
    try {
        return std::stoul(value);
    } catch (const std::exception& e) {
        error(translate("ERROR_SIZE_T_CONVERT", value));
    }
    return 0;
}

template<>
int ConfigParser::_convertValue_<int>(const std::string& value) const {
    try {
        return std::stoi(value);
    } catch (const std::exception& e) {
        error(translate("ERROR_INTEGER_CONVERT", value));
    }
    return 0;
}

template<>
bool ConfigParser::_convertValue_<bool>(const std::string& value) const {
    int ivalue = _convertValue_<int>(value);
    return (ivalue == 1) ? true : false;
}

template<>
float ConfigParser::_convertValue_<float>(const std::string& value) const {
    try {
        return std::stof(value);
    } catch (const std::exception& e) {
        error(translate("ERROR_FLOAT_CONVERT", value));
    }
    return 0.;
}

template<>
std::string ConfigParser::_convertValue_<std::string>(const std::string& value) const {
    return value;
}

//
// Public methods
//

void ConfigParser::loadFromFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(mutex);
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        error(translate("ERROR_OPEN_FILE",filename));
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
            key = str::lowercase( trim(line.substr(0, pos)) );
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
template std::size_t ConfigParser::getValue<std::size_t>(const std::string&) const;
template int ConfigParser::getValue<int>(const std::string&) const;
template bool ConfigParser::getValue<bool>(const std::string&) const;
template std::string ConfigParser::getValue<std::string>(const std::string&) const;

void ConfigParser::printConfig() const {
    for (const auto& [key, value] : config_map) {
        std::cout << key << " = " << value << std::endl;
    }
}