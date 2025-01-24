#pragma once

#include "Collections.h"
#include "DataManager.h"
#include "Environment.h"

namespace amech {

    class MechanicalProblem {

        protected :
            /// @brief Collection of physical data readed from ressources files.
            /// @details This collection is used to store all the physical data:
            ///  - materials
            ///  - fatigue laws
            ///  - plate coefficients
            std::shared_ptr<adata::Collections> physical_data = nullptr;
            /// @brief Input data readed from the user input files.
            std::shared_ptr<adata::DataManager> input_data = nullptr;


        public :

            MechanicalProblem() = default;
            virtual ~MechanicalProblem() = default;

            /// @brief Initialize the mechanical problem.
            void init();

            /// @brief Set the physical data collection.
            void set_physical_data();

            /// @brief Read the user input data.
            void read_input_data();

            //virtual void solve() = 0;

    };



}