#include <iostream>
#include <filesystem>
#include <sstream>
#include <stdexcept>

#include "LocalPath.h"
#include "TranslationManager.h"

#include "yaml-cpp/yaml.h"

namespace fs = std::filesystem;
using namespace abase;

// Initialize global instance
namespace abase {
    TranslationManager globalTranslationManager;
}

/// @brief Check if the YAML node used the expected `format_type`
/// @param node YAML node
/// @return true is the `format_type` is correct
bool dictionnary_format(const YAML::Node& node, const std::string& format_type) {
    for (const auto& key : node) {
        if (key.first.as<std::string>() == format_type) {
            try {
                bool status = key.second.as<bool>();
                return status;
            }
            catch(std::exception const& e)
            {
                return false;
            } 
        }
    }
    return false;
}

void TranslationManager::print() const {
    for (const auto& dic_key : translations) {
        std::string msg = dic_key.first + ":";
        for (const auto& value : dic_key.second) {
            msg += "\n  " + value.first + ": " + value.second;
        }
        std::cout << msg << std::endl;
    }
}

void TranslationManager::loadTranslationsFromFile(const std::string& filename) {
    // Check the file type and status before loading
    if ( !is_file_readable(filename) ) {
        throw std::invalid_argument("Cannot read the file " + filename + " !");
    }

    YAML::Node node = YAML::LoadFile(filename);
    if (!dictionnary_format(node, format_type)) return;

    for (const auto& key : node) {
        std::string dic_key = key.first.as<std::string>();
        if (dic_key == format_type) continue;
        
        // Check previous definition
        if (translations.find(dic_key) != translations.end()) {
            throw std::invalid_argument("The key " + dic_key + " is already defined !");
        } 

        std::unordered_map<std::string, std::string> dic_values;
        for (const auto& subkey : key.second) {
            std::string lang = subkey.first.as<std::string>();
            std::string value = subkey.second.as<std::string>();
            dic_values.insert({lang, value});
        }
        translations.insert({dic_key, dic_values});
    }
}

void TranslationManager::loadAllTranslations(const std::string& directory) {
    std::vector<std::string> files = getFilesbyExtension(directory, file_extension);
    for (const auto& ff : files) {
        loadTranslationsFromFile(ff);
    }
}

void TranslationManager::setCurrentLanguage(const std::string& langCode) {
    currentLanguage = langCode;
}

std::string TranslationManager::replacePlaceholders(const std::string& message, const std::vector<std::string>& values) const {
    std::string result = message;
    for (size_t i = 0; i < values.size(); ++i) {
        std::string placeholder = "{" + std::to_string(i) + "}";
        size_t pos;
        while ((pos = result.find(placeholder)) != std::string::npos) {
            result.replace(pos, placeholder.length(), values[i]);
        }
    }
    return result;
}

std::string TranslationManager::translate(const std::string& key) const {
    if (translations.empty()) return key;
    
    auto it = translations.find(key);
    if (it == translations.end()) return key;
    
    auto jt = it->second.find(currentLanguage);
    return jt != it->second.end() ? jt->second : key;
}

std::string TranslationManager::translate(const std::string& key, const std::string& value) const {
    return translate(key, std::vector<std::string>{value});
}

std::string TranslationManager::translate(const std::string& key, const std::vector<std::string>& values) const {
    std::string translated_key = translate(key);
    return replacePlaceholders(translated_key, values);
}
