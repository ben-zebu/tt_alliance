#include "TranslationManager.h"
#include <iostream>
#include <sstream>
#include <filesystem>

#include "tinyxml2.h"

namespace fs = std::filesystem;

// Initialize global instance
TranslationManager globalTranslationManager;

void TranslationManager::loadTranslationsFromFile(const std::string& filename) {
    using namespace tinyxml2;
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) {
        std::cerr << "Error loading file: " << filename << std::endl;
        return;
    }

    XMLElement* root = doc.FirstChildElement("translations");
    if (!root) {
        std::cerr << "No root element found." << std::endl;
        return;
    }

    XMLElement* language = root->FirstChildElement("language");
    while (language) {
        const char* langCode = language->Attribute("code");
        if (langCode) {
            XMLElement* translation = language->FirstChildElement("translation");
            while (translation) {
                const char* key = translation->Attribute("key");
                const char* value = translation->GetText();
                if (key && value) {
                    translations[langCode][key] = value;
                }
                translation = translation->NextSiblingElement("translation");
            }
        }
        language = language->NextSiblingElement("language");
    }
}

void TranslationManager::loadAllTranslations(const std::string& directory) {
    std::vector<std::string> filePaths;
    searchXmlFiles(directory, filePaths);

    for (const auto& filePath : filePaths) {
        loadTranslationsFromFile(filePath);
    }
}

void TranslationManager::searchXmlFiles(const std::string& directory, std::vector<std::string>& filePaths) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.path().extension() == ".xml") {
            filePaths.push_back(entry.path().string());
        }
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
    auto it = translations.at(currentLanguage).find(key);
    return it != translations.at(currentLanguage).end() ? it->second : key;
}

std::string TranslationManager::translate(const std::string& key, const std::string& value) const {
    return translate(key, std::vector<std::string>{value});
}

std::string TranslationManager::translate(const std::string& key, const std::vector<std::string>& values) const {
    auto it = translations.at(currentLanguage).find(key);
    if (it != translations.at(currentLanguage).end()) {
        return replacePlaceholders(it->second, values);
    }
    return key;
}
