#pragma once

#include "Commands.h"
#include "Environment.h"
#include "FileReader.h"

namespace adata::parts {

    /// @brief Class to store coefficients for perforated plates. Coefficients are stored for one unique value of 
    // \f$ P/h \f$ ratio. 
    class PlateCoefficients {
        private :      

        public:
            /// @brief \f$ P/h \f$ ratio
            double Ph = 0.;
            /// @brief list of angles values
            std::vector<double> angles;
            /// @brief list of \f$ a_{phi} \f$ coefficients
            std::vector<double> a_phi;
            /// @brief list of \f$ b_{phi} \f$ coefficients
            std::vector<double> b_phi;
            /// @brief list of \f$ c_{phi} \f$ coefficients
            std::vector<double> c_phi;

            /// @brief Clear the object values
            void clear();

            PlateCoefficients() = default;
            virtual ~PlateCoefficients() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            void init(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Verify the coherence of object values
            /// @param filecontext file content
            void verify(const std::string& filecontext) const;
    };


}