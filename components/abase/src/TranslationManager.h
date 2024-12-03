#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace abase {

    class TranslationManager {
        private:
            /// @brief Standard extension for YAML files
            std::string file_extension = ".yml";

            /// @brief Key used to detect the YAML format
            std::string format_type = "DICTIONNARY_FORMAT";

            /// @brief Replace place holder strings (such as {0}, {1}, ...) by an other string
            /// @param message initial message with place holder
            /// @param values  list of string to replace place holder
            /// @return the modified message
            std::string replacePlaceholders(const std::string& message, const std::vector<std::string>& values) const;

        protected:
            /// @brief Dictionnary used for the transalation
            /// Data are stored in the following order (assuming an Python representation)
            /// @code
            ///     translation[key] = { 'en': en_msg, 'fr': fr_msg}
            /// @endcode
            std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations;
            /// @brief Current language
            std::string currentLanguage = "en";

        public:
            /// @brief Default constructor
            TranslationManager() = default;

            /// @brief print all keys from the translation manager
            void print() const;
            
            /// @brief Complete the translation dictionnary from a file
            /// @param filename for translation dictionnary in YAML format
            void loadTranslationsFromFile(const std::string& filename);
            /// @brief Complete the translation dictionnary from all YAML files contained in a directory
            /// @param directory where all YAML files are stored
            void loadAllTranslations(const std::string& directory);

            /// @brief Set the output language
            /// @param langCode the output language
            void setCurrentLanguage(const std::string& langCode);

            /// @brief function used to return the translation associated to a key and for the current language
            /// @param key used to search in translatation dictionnary
            /// @return a string associated to the key and the current language
            std::string translate(const std::string& key) const;
            /// @brief function used to return the translation associated to a key and for the current language
            /// @param key used to search in translatation dictionnary
            /// @param value used to complete the output message with a string
            /// @return a string associated to the key and the current language
            std::string translate(const std::string& key, const std::string& value) const;
            /// @brief function used to return the translation associated to a key and for the current language
            /// @param key used to search in translatation dictionnary
            /// @param values used to complete the output message with a list of string
            /// @return a string associated to the key and the current language
            std::string translate(const std::string& key, const std::vector<std::string>& values) const;

    };

    /// @brief Reference to the global translation manager
    extern TranslationManager globalTranslationManager;

} // namespace abase


/// @brief Alias function used to link with the global translation manager
/// @param key used to search in translatation dictionnary
/// @return a string associated to the key and the current language
inline std::string translate(const std::string& key) {
    return abase::globalTranslationManager.translate(key);
}

/// @brief Alias function used to link with the global translation manager
/// @param key used to search in translatation dictionnary
/// @param value used to complete the output message with a string
/// @return a string associated to the key and the current language
inline std::string translate(const std::string& key, const std::string& value) {
    return abase::globalTranslationManager.translate(key, value);
}

/// @brief Alias function used to link with the global translation manager
/// @param key used to search in translatation dictionnary
/// @param values used to complete the output message with a list of string
/// @return a string associated to the key and the current language
inline std::string translate(const std::string& key, const std::vector<std::string>& values) {
    return abase::globalTranslationManager.translate(key, values);
}

