#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata::parts {

    class ProblemLoadstep {
        private:
            /// @brief Mismatch between the number of external torsors and the number of coefficients
            bool _mismatch_coefficients = false;
            /// @brief Number of external torsors
            std::size_t _nb_torsors = 0;

        public:
            /// @brief Card ranks that contain stress values associated to pressure loads
            std::vector<std::size_t> pressure_cards;
            /// @brief pressure values associated to the pressure cards
            std::vector<double> pressure_values;
            /// @brief Card rank that contains stress values associated to temperature loads
            std::size_t thermal_card = 0;
            /// @brief Thermomechanical coefficient associated to the thermal card
            double kh = 1.0;
            /// @brief Maximum coefficients associated to external torsors, case without earthquake
            std::vector<double> max_ef;
            /// @brief Minimum coefficients associated to external torsors, case without earthquake
            std::vector<double> min_ef;
            /// @brief Maximum coefficients associated to external torsors, case with earthquake
            std::vector<double> max_ft;
            /// @brief Minimum coefficients associated to external torsors, case with earthquake
            std::vector<double> min_ft;

            ProblemLoadstep() = default;
            virtual ~ProblemLoadstep() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            /// @param nb_torsors number of external torsors
            void init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t nb_torsors);

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            void verify(const std::string& filecontext) const;
    };

}