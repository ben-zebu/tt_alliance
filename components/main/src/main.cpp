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
#include "StressStates.h"
#include "Stress.h"
#include "Table.h"
#include "Coefficient.h"

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

StressStates generateStressStates(int numStates) {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Define the range for the random values
    const double maxValue = 300.0;
    const double minValue = -1.*maxValue;

    StressStates states = StressStates();

    for (int i = 0; i < numStates; ++i) {
        std::vector<double> components(6);
        for (int j = 0; j < 6; ++j) {
            components[j] = minValue + (static_cast<double>(rand()) / RAND_MAX) * (maxValue - minValue);
        }
        states.addState(components);
    }

    return states;
}

double computeStressIntensity(const StressStates& states, const std::string& method, const size_t interations) {
    double Smax = 0.0;
    for (size_t iter = 0; iter < interations; ++iter) {
        for (size_t i = 0; i < states.size(); ++i) {
            if (method == "tresca") {
                Smax = std::max(Smax, states.tresca(i));
            } else if (method == "trescaFast") {
                Smax = std::max(Smax, states.trescaFast(i));
            } else if (method == "trescaWithCardan") {
                Smax = std::max(Smax, states.trescaWithCardan(i));
            } else if (method == "mises") {
                Smax = std::max(Smax, states.mises(i));
            } else if (method == "misesReduced") {
                if (states.misesReduced(i)> Smax) {
                    Smax = std::max(Smax, states.tresca(i));
                }
            }
        }
    }
    return Smax;
}

void performBenchmark(const StressStates& states, const std::string& method, const int interations) {
    start_timer(method);
    std::cout << "Benchmarking " << method << " method..." << std::endl;
    double Smax = computeStressIntensity(states, method, interations);
    std::cout << "Max stress intensity for " << method << ": " << Smax << std::endl;
    stop_timer(method);
    print_timer(method);
    std::cout << std::endl;
}

void benchmark() {
    const size_t numStates = 1E5;
    const size_t iterations = 100;
    std::cout << "Number of operations = " << numStates * iterations << std::endl;
    std::cout << std::endl;

    auto stressStates = generateStressStates(numStates);
    
    std::vector<std::string> methods = {"tresca", "trescaFast", "trescaWithCardan", "mises", "misesReduced"};
    for (const auto& method : methods) {
        performBenchmark(stressStates, method, iterations);
    }
}

int main(int argc, char* argv[]) {
    //start_timer("main");

    // auto [path, exe] = splitExecutablePath(argv[0]);
    // std::cout << "Path: " << path << std::endl;
    // std::cout << "Executable: " << exe << std::endl;

    // std::string configFile = path + "/../etc/alliance.conf";
    // abase::globalConfigParser.loadFromFile(configFile);
    // std::cout << "number of keys: " << abase::globalConfigParser.getKeyCount() << std::endl;
    // //globalConfigParser.printConfig();

    // std::string ressourcesPath = path + "/../ressources";
    // abase::globalTranslationManager.loadAllTranslations(ressourcesPath);
    // abase::globalTranslationManager.setCurrentLanguage("fr");

    //float version = get_parser_value<float>("project");

    //greetUser();
    //showTemperature();
    //sayFarewell();

    //stop_timer("main");
    //print_timer("main");
    amath::Stress s1({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    amath::Stress s2(2*s1);
    amath::Stress s3;

    std::cout << "s1 = " << s1.get_components() << std::endl;
    std::cout << "s2 = " << s2.get_components() << std::endl;
    s3 = {100.0, 200.0, 50.0, -20.0, 10.0, 5.0};
    std::cout << "s3 = " << s3.get_components() << std::endl;
    std::cout << "tresca(s3) = " << s3.tresca() << " mises(s3) = " << s3.mises() << std::endl;

    std::vector<double> x = {0.0, 10.,  100.0};
    std::vector<double> y = {0.0, 100., 200.0};
    amath::Table table(x, y);
    amath::Table table2;
    std::cout << "table.get_xrange() = ";
    for (auto val : table.get_xrange() ) {  std::cout << val << " "; }
    std::cout << std::endl;
    std::cout << "table.get_yrange() = ";
    for (auto val : table.get_yrange() ) {  std::cout << val << " "; }
    std::cout << std::endl;
    std::cout << "table.get_yvalue(5.0) = " << table.get_yvalue(5.0, "linear") << std::endl;
    std::cout << "table.get_yvalue(50.0) = " << table.get_yvalue(50.0, "linear") << std::endl;

    table2 = table;
    std::cout << "table2 = table" << std::endl;
    std::cout << "table2.get_xmax() = " << table2.get_xmax() << std::endl;

    amath::LinearCoefficient Sm(table);
    std::cout << "Sm(5.0) = " << Sm.get_yvalue(5.0) << std::endl;
    amath::LogarithmicCoefficient Law(table);
    std::cout << "Law(50.0) = " << Law.get_yvalue(50.0) << std::endl;
    amath::ConstantCoefficient Cm(145.0);
    std::cout << "Cm = " << Cm.get() << std::endl;


    return 0;
}