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

#include "GlobalTimer.h"
#include "StressStates.h"
#include "Stress.h"
#include "Table.h"
#include "Coefficient.h"
#include "Combination.h"

amath::StressStates generateStressStates(int numStates) {
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Define the range for the random values
    const double maxValue = 300.0;
    const double minValue = -1.*maxValue;

    amath::StressStates states = amath::StressStates();

    for (int i = 0; i < numStates; ++i) {
        std::array<double, amath::STRESS_SIZE> components;
        for (std::size_t j = 0; j < components.size(); ++j) {
            components[j] = minValue + (static_cast<double>(rand()) / RAND_MAX) * (maxValue - minValue);
        }
        states.add_stress( amath::Stress(components) );
    }

    return states;
}

void performBenchmark(amath::StressStates& states, const std::string& method, const int interations) {
    std::vector<size_t> states_ids;
    for (size_t i = 0; i < states.size(); ++i) { states_ids.push_back(i); }
    amath::TriangularCombination explorer(states_ids);
    
    states.set_equivalent_stress_method(method);    
    
    start_timer(method + " loop");
    std::cout << "Benchmarking " << method << " method with states_id loops ..." << std::endl;
    for (int i = 0; i < interations; ++i) {
        auto Smax = states.stress_range(states_ids);
        if (i == interations - 1) {
            std::cout << "Max stress intensity for " << method << ": " << Smax.Sr << std::endl;
        }
    }
    stop_timer(method + " loop");
    print_timer(method + " loop");
    std::cout << std::endl;
    
    start_timer(method + " explorer");
    std::cout << "Benchmarking " << method << " method with explorer ..." << std::endl;
    for (int i = 0; i < interations; ++i) {
        auto Smax = states.stress_range(explorer);
        if (i == interations - 1) {
            std::cout << "Max stress intensity for " << method << ": " << Smax.Sr << std::endl;
        }
    }
    stop_timer(method + " explorer");
    print_timer(method + " explorer");
    std::cout << std::endl;
}

void benchmark() {
    //const size_t numStates = 2E4;
    const size_t numStates = 1000;
    const size_t iterations = 5;

    const size_t numOperations = int(numStates*(numStates-1) / 2 * iterations);
    std::cout << "Number of operations = " << numOperations << std::endl;
    std::cout << std::endl;

    auto stressStates = generateStressStates(numStates);
    
    //std::vector<std::string> methods = {"tresca", "mises", "reduced_mises"};
    std::vector<std::string> methods = {"tresca", "reduced_mises"};
    for (const auto& method : methods) {
        performBenchmark(stressStates, method, iterations);
    }
}

int main(int argc, char* argv[]) {
    benchmark();
    return 0;
}