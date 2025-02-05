#pragma once

#include "Combination.h"
#include "DataManager.h"
#include "Environment.h"

namespace amech {

    class TransientCombination {
        protected :
            /// @brief Input data readed from the user input files.
            std::shared_ptr<adata::DataManager> input_data = nullptr;

        public :
            /// @brief Constructor by default.
            TransientCombination() = default;
            /// @brief Constructor with input data.
            /// @param input_data Input data readed from the user input files.
            TransientCombination(std::shared_ptr<adata::DataManager> input_data) : input_data(input_data) {};
            /// @brief Destructor.
            virtual ~TransientCombination() = default;

            /// @brief Create a combination generator (upper triangular form) for all transient's time steps.
            /// @param transient name of the transient
            /// @return combination generator
            amath::TriangularCombination transient_combination(const std::string& transient) const;
            /// @brief Create a combination generator (rectangular form) for all time steps of two transients. 
            /// Combinations of two time steps from one transient are excluded.
            /// @param first_transient name of the first transient
            /// @param second_transient name of the second transient
            /// @return combination generator
            amath::RectangularCombination crossed_combination(const std::string& first_transient, const std::string& second_transient) const;
            /// @brief Create a combination generator (rectangular form) for all transient's time steps. Each 
            /// combination includes a specific time steps
            /// @param transient name of the transient
            /// @param pivot_rank time step rank to include in each combination
            /// @return combination generator
            amath::RectangularCombination fictive_combination(const std::string& transient, std::size_t pivot_rank) const;
    };
    
}