#include <iostream>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <limits.h>
    #include <stdlib.h>
#endif
#include <string>
#include <utility>

#include "ConfigParser.h"
#include "TranslationManager.h"
#include "GlobalTimer.h"


// Function to get current date
std::string getCurrentDate() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localTime);
    return buffer;
}

// Function to get current time
std::string getCurrentTime() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
    return buffer;
}

void greetUser() {
    std::string currentDate = getCurrentDate();
    std::string currentTime = getCurrentTime();
    std::cout << translate("greeting", {currentDate, currentTime}) << std::endl;
}

void sayFarewell() {
    std::cout << translate("farewell") << std::endl;
}

void showTemperature() {
    std::string currentDate = getCurrentDate();
    std::string currentTemperature = "20"; // Example temperature, can be replaced with actual value
    std::cout << translate("temperature", currentTemperature) << std::endl;
}

std::string getExecutablePath(char* argv0) {
    #ifdef _WIN32
        char full_path[MAX_PATH];
        GetFullPathName(argv0, MAX_PATH, full_path, nullptr);
        return std::string(full_path);
    #else
        char full_path[PATH_MAX];
        realpath(argv0, full_path);
        return std::string(full_path);
    #endif
}

std::pair<std::string, std::string> splitExecutablePath(char* argv0) {
    std::string fullPath = getExecutablePath(argv0);
    
    size_t lastSeparator = fullPath.find_last_of("/\\");
    
    std::string path = fullPath.substr(0, lastSeparator);
    std::string exe = fullPath.substr(lastSeparator + 1);
    
    return {path, exe};
}


int main(int argc, char* argv[]) {
    start_timer("main");

    auto [path, exe] = splitExecutablePath(argv[0]);
    std::cout << "Path: " << path << std::endl;
    std::cout << "Executable: " << exe << std::endl;

    std::string configFile = path + "/../etc/alliance.conf";
    abase::globalConfigParser.loadFromFile(configFile);
    std::cout << "number of keys: " << abase::globalConfigParser.getKeyCount() << std::endl;
    //globalConfigParser.printConfig();

    std::string ressourcesPath = path + "/../ressources";
    abase::globalTranslationManager.loadAllTranslations(ressourcesPath);
    abase::globalTranslationManager.setCurrentLanguage("fr");

    //float version = get_parser_value<float>("project");

    greetUser();
    showTemperature();
    sayFarewell();

    stop_timer("main");
    print_timer("main");
    
    return 0;
}
