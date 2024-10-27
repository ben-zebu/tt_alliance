#pragma once

#include <string>
#include <unordered_map>

/**
 * @brief A class for parsing and managing configuration settings
 * 
 * ConfigParser provides functionality to load, store, and retrieve configuration values
 * from files and memory. It supports various data types through templated methods.
 * 
 * @note This class is not thread-safe.
 */
class ConfigParser {
private:
    std::unordered_map<std::string, std::string> config_map;

    template<typename T>
    T _convertValue_(const std::string& value) const;
    std::string _findValue_(const std::string& key) const;

    std::string trim(const std::string& str) const;

public:
    ConfigParser() = default;
    
    void loadFromFile(const std::string& filename);

    template<typename T>
    T getValue(const std::string& key) const;

    void setValue(const std::string& key, const std::string& value);

    int getKeyCount() const {
        return config_map.size();
    }

    void printConfig() const;

};

// Global instance of ConfigParser
extern ConfigParser globalConfigParser;

template<typename T>
inline T get_parser_value(const std::string& key) {
    return globalConfigParser.getValue<T>(key);
}

template<typename T>
inline void set_parser_value(const std::string& key, const T& value) {
    globalConfigParser.setValue(key, std::to_string(value));
}
