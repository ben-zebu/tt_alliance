#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>

// #ifdef _WIN32
//     #include <windows.h>
// #else
//     #include <limits.h>
//     #include <stdlib.h>
// #endif
#include <memory>
#include <string>
#include <utility>

#include "GlobalTimer.h"
#include "Filesystem.h"
#include "StressStates.h"
#include "Stress.h"
#include "Table.h"
#include "Coefficient.h"
#include "Combination.h"

#include "Environment.h"
#include "FileLogger.h"
#include "ConfigParser.h"
#include "CommandsCollector.h"

#include "DataManager.h"
#include "FatigueLawCollector.h"
#include "BaseMaterialCollector.h"
#include "Initiate.h"

amath::StressStates generateStressStates(const std::size_t numStates, const std::size_t numTorsors) {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Define the range for the random values
    const double maxValue = 100.0;
    const double minValue = -1.*maxValue;
    const double maxCoef = 10.0;
    const double minCoef = -1.*maxCoef;

    amath::StressStates states = amath::StressStates();
    std::array<double, amath::STRESS_SIZE> components;
    std::vector<double> cmax(numTorsors);
    std::vector<double> cmin(numTorsors);

    // Set Stress components associated to each torsor
    for (std::size_t i = 0; i < numTorsors; ++i) {
        for (std::size_t j = 0; j < components.size(); ++j) {
            components[j] = minValue + (static_cast<double>(rand()) / RAND_MAX) * (maxValue - minValue);
        }
        states.add_torsor( amath::Stress(components) );
    }

    // Set Stress components and torsor coefficients for each state
    for (std::size_t i = 0; i < numStates; ++i) {
        // set stess components
        for (std::size_t j = 0; j < components.size(); ++j) {
            components[j] = minValue + (static_cast<double>(rand()) / RAND_MAX) * (maxValue - minValue);
        }
        states.add_stress( amath::Stress(components) );
    
        // set torsor coefficients
        for (std::size_t j = 0; j < numTorsors; ++j) {
            cmax[j] = minCoef + (static_cast<double>(rand()) / RAND_MAX) * (maxCoef - minCoef);
            cmin[j] = (i != 2) ? minCoef + (static_cast<double>(rand()) / RAND_MAX) * (maxCoef - minCoef) : cmax[i];
        }
        states.add_torsor_coefficients(cmax, cmin);
    }

    states.check_integrity();
    return states;
}

void performBenchmark(amath::StressStates& states, const std::string& method, const int interations) {
    std::vector<size_t> states_ids;
    for (size_t i = 0; i < states.size(); ++i) { states_ids.push_back(i); }
    amath::TriangularCombination explorer(states_ids);
    
    states.set_equivalent_stress_method(method);    
    
    start_timer(method);
    std::cout << "Benchmarking " << method << " method with explorer ..." << std::endl;
    for (int i = 0; i < interations; ++i) {
        auto Smax = states.stress_range(explorer);
        if (i == interations - 1) {
            auto Sr = Smax.get_range();
            std::cout << "Max stress intensity for " << method << ": " << Sr.range << std::endl;
        }
    }
    stop_timer(method);
    print_timer(method);
    std::cout << std::endl;
}

void benchmark() {
    //const size_t numStates = 2E4;
    const size_t numStates = 1000;
    const size_t numTorsors = 4;
    const size_t iterations = 1;

    const size_t numOperations = int(numStates*(numStates-1) / 2 * std::pow(2, numTorsors - 1) * iterations);
    std::cout << "Number of operations = " << numOperations << std::endl;
    std::cout << std::endl;

    auto stressStates = generateStressStates(numStates, numTorsors);
    
    //std::vector<std::string> methods = {"tresca", "mises", "reduced_mises"};
    std::vector<std::string> methods = {"tresca", "reduced_mises"};
    for (const auto& method : methods) {
        performBenchmark(stressStates, method, iterations);
    }
}

void yaml_test() {
    std::string input_file = get_parser_value<std::string>("input_file");
    std::string app_path = get_parser_value<std::string>("application_path");

    std::string material_commands = app_path + "/" +  get_parser_value<std::string>("material_commands");
    std::string material_files = get_parser_value<std::string>("material_files");
    std::string fatigue_law_files = get_parser_value<std::string>("fatigue_law_files");

    adata::FatigueLawCollector lawCollector;
    std::vector<std::string> files = str::split(str::replace(fatigue_law_files, ",", " "));
    for (const auto& file : files) {
        lawCollector.read_data(app_path + "/" + file, material_commands);
    }

    adata::BaseMaterialCollector materialCollector;
    files = str::split(str::replace(material_files, ",", " "));
    for (const auto& file : files) {
        materialCollector.read_data(app_path + "/" + file, material_commands);
    }

    std::string input_commands = app_path + "/" +  get_parser_value<std::string>("input_commands");
    adata::DataManager dataManager;
    dataManager.read_data(input_file, input_commands);
    dataManager.verify();
}

int main(int argc, char* argv[]) {
    // initialization process
    init::start(argc, argv);

    yaml_test();
    //benchmark();
    return 0;
}