#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <string>
#include <unordered_map>
#include <vector>
//~ #include "tinyxml2.h"

class TranslationManager {
public:
    TranslationManager() = default;
    void loadTranslationsFromFile(const std::string& filename);
    void loadAllTranslations(const std::string& directory);
    void setCurrentLanguage(const std::string& langCode);
    std::string translate(const std::string& key) const;
    std::string translate(const std::string& key, const std::string& value) const;
    std::string translate(const std::string& key, const std::vector<std::string>& values) const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations;
    std::string currentLanguage;
    void searchXmlFiles(const std::string& directory, std::vector<std::string>& filePaths);
    std::string replacePlaceholders(const std::string& message, const std::vector<std::string>& values) const;
};

extern TranslationManager globalTranslationManager;

// Short alias for translate function
inline std::string translate(const std::string& key) {
    return globalTranslationManager.translate(key);
}

inline std::string translate(const std::string& key, const std::string& value) {
    return globalTranslationManager.translate(key, value);
}

inline std::string translate(const std::string& key, const std::vector<std::string>& values) {
    return globalTranslationManager.translate(key, values);
}

#endif // TRANSLATIONMANAGER_H
