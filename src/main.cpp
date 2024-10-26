#include <iostream>
#include <ctime>
#include "TranslationManager.h"

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

int main() {
    globalTranslationManager.loadAllTranslations("ressources");
    globalTranslationManager.setCurrentLanguage("fr");

    greetUser();
    showTemperature();
    sayFarewell();

    return 0;
}
