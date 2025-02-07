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
            /// @param trk transient rank
            /// @return combination generator
            amath::TriangularCombination transient_combination(std::size_t trk) const;
            /// @brief Create a combination generator (rectangular form) for all time steps of two transients. 
            /// Combinations of two time steps from one transient are excluded.
            /// @param trk1 rank of the first transient
            /// @param trk2 rank of the second transient
            /// @return combination generator
            amath::RectangularCombination crossed_combination(std::size_t trk1, std::size_t trk2) const;
            /// @brief Create a combination generator (rectangular form) for all transient's time steps. Each 
            /// combination includes a specific time steps
            /// @param trk transient rank
            /// @param pivot_rank time step rank to include in each combination
            /// @return combination generator
            amath::RectangularCombination fictive_combination(std::size_t trk, std::size_t pivot_rank) const;


            /// @brief Return the list of common groups of two transients
            /// @param trk1 rank of the first transient
            /// @param trk2 rank of the second transient
            /// @return list of common groups
            std::vector<std::string> common_groups(std::size_t trk1, std::size_t trk2) const;
            /// @brief Return the list of crossing transient that can be used to combine the transient
            /// @param trk1 rank of the first transient
            /// @param trk2 rank of the second transient
            /// @return list of crossing transients
            std::vector<std::string> crossing_transients(std::size_t trk1, std::size_t trk2) const;
            /// @brief Check if two transients are in the same shared group
            /// @param trk1 rank of the first transient
            /// @param trk2 rank of the second transient
            /// @return true if the two transients are in the same shared group
            bool is_shared_group(std::size_t trk1, std::size_t trk2) const;

    };
    
}