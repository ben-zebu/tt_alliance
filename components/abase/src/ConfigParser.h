#pragma once

#include <mutex>
#include <string>
#include <unordered_map>


namespace abase {

    /**
     * @brief A thread-safe configuration parser class that manages key-value pairs
     * 
     * The ConfigParser class provides functionality to load, store, and retrieve
     * configuration values from a file. It supports different data types through
     * template methods and ensures thread safety using mutex locks.
     * 
     * Key features:
     * - Thread-safe operations
     * - Type-safe value retrieval
     * - File-based configuration loading
     * - Key-value pair management
     * 
     * Usage example:
     * @code
     * ConfigParser config;
     * config.loadFromFile("config.txt");
     * int value = config.getValue<int>("some_key");
     * config.setValue("new_key", "value");
     * @endcode
     * 
     * @note All public methods are thread-safe
     * @see getValue(), setValue(), loadFromFile()
     */
    class ConfigParser {
    private:
        /// @brief Map of key-value pairs
        std::unordered_map<std::string, std::string> config_map;
        /// @brief convert a string value to a specific type
        /// @tparam T expected type
        /// @param value string value to convert
        /// @return a value of type T
        template<typename T>
        T _convertValue_(const std::string& value) const;
        /// @brief Get value associated to a key
        /// @param key string key
        /// @return string value
        std::string _findValue_(const std::string& key) const;
        /// @brief Trim leading and trailing spaces from a string
        std::string trim(const std::string& str) const;

        /// @brief Mutex to ensure thread safety
        std::mutex mutex;

    public:
        /// @brief Default constructor
        ConfigParser() = default;
        /// @brief Load configuration from a file
        /// @param filename path to the configuration file            
        void loadFromFile(const std::string& filename);
        /// @brief Get the value associated to a key (public method)
        /// @tparam T expected type
        /// @param key string key
        /// @return converted value of type T
        template<typename T>
        T getValue(const std::string& key) const;
        /// @brief Set a key-value pair
        /// @param key key string
        /// @param value value string
        void setValue(const std::string& key, const std::string& value);
        /// @brief Get the number of key-value pairs
        int getKeyCount() const {
            return config_map.size();
        }

        void printConfig() const;

    };

    // Global instance of ConfigParser
    extern abase::ConfigParser globalConfigParser;

} //  namespace abase

/// @brief Generic function to get a value from the global configuration parser
/// @tparam T Type of the value to retrieve
/// @param key name of the key to retrieve
/// @return value associated to the key
template<typename T>
inline T get_parser_value(const std::string& key) {
    return abase::globalConfigParser.getValue<T>(key);
}
/// @brief Generic function to set a value in the global configuration parser
/// @param key name of the key to set
/// @param value associated value to the key
inline void set_parser_value(const std::string& key, const std::string& value) {
    abase::globalConfigParser.setValue(key, value);
}

