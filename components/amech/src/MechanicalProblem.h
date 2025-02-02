#pragma once

#include "Collections.h"
#include "DataManager.h"
#include "Environment.h"
#include "OutputResume.h"

namespace amech {

    class MechanicalProblem {
        private :
            /// @brief Set output resume file and folder.
            void init_output_resume();
            /// @brief Set the physical data collection.
            void set_physical_data();
            /// @brief Read the user input data.
            void read_input_data();

        protected :
            /// @brief Collection of physical data readed from ressources files.
            /// @details This collection is used to store all the physical data:
            ///  - materials
            ///  - fatigue laws
            ///  - plate coefficients
            std::shared_ptr<adata::Collections> physical_data = nullptr;
            /// @brief Input data readed from the user input files.
            std::shared_ptr<adata::DataManager> input_data = nullptr;

            /// @brief Output resume file and folder.
            OutputResume output_resume;

        public :
            MechanicalProblem() = default;
            virtual ~MechanicalProblem() = default;

            /// @brief Initialize the mechanical problem.
            void init();
            /// @brief Verify the integrity of the mechanical problem.
            void verify();
            /// @brief Solve the mechanical problem.
            void solve();
            /// @brief Close the mechanical problem.
            void close();

    };

}
