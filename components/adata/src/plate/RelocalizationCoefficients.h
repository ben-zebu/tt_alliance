#pragma once

#include "Environment.h"
#include "Table.h"

namespace adata {

    class RelocalizationCoefficients {
        protected :
            /// @brief Return the relocalisation coefficient associated to a given angle
            /// @param angle given angle
            /// @param type type of relocalisation coefficient "a_phi", "b_phi" or "c_phi" 
            /// @return Value of relocalisation coefficient
            double get_relocalization_coef(const double& angle, const std::string& type) const;

            /// @brief Values of relocalisation coefficients for a list of angles. The possible
            /// keys are: "a_phi", "b_phi" or "c_phi" 
            std::unordered_map<std::string, amath::Table> _coefficients_;


        public:

            /// @brief Set values of \f$ a_{\phi} \f$ for several angles
            void set_a_phi(std::vector<double> angles, std::vector<double> coefs);
            /// @brief Set values of \f$ b_{\phi} \f$ for several angles
            void set_b_phi(std::vector<double> angles, std::vector<double> coefs);
            /// @brief Set values of \f$ c_{\phi} \f$ for several angles
            void set_c_phi(std::vector<double> angles, std::vector<double> coefs);

            /// @brief Get value of \f$ a_{\phi} \f$ for a angle
            /// @param angle given angle
            /// @return Value of \f$ a_{\phi}
            double get_a_phi(const double& angle) const;
            /// @brief Get value of \f$ b_{\phi} \f$ for a angle
            /// @param angle given angle
            /// @return Value of \f$ b_{\phi}            
            double get_b_phi(const double& angle) const;
            /// @brief Get value of \f$ c_{\phi} \f$ for a angle
            /// @param angle given angle
            /// @return Value of \f$ c_{\phi}            
            double get_c_phi(const double& angle) const;

    };
}