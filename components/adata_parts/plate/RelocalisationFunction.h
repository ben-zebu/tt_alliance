#pragma once

#include "Environment.h"
#include "Table.h"

#include "PlateCoefficients.h"

namespace adata::parts {

    /// @brief Class to store relocalisation coefficients for perforated plates
    class RelocalisationFunction {
        private:
            /// @brief Build interpolated values for a given type of relocalisation coefficient (a_phi, b_phi or c_phi)
            /// @param coef_type relocalisation coefficient type (a_phi, b_phi or c_phi)
            /// @param reloc_f1 Plate relocalisation function for the first P/h ratio
            /// @param reloc_f2 Plate relocalisation function for the second P/h ratio
            /// @param merged_angles Output list of angles values
            /// @param ph_ratio \f$ P/h \f$ ratio used to interpolate values
            /// @return interpolated relolcalisation coefficients a_phi, b_phi or c_phi
            std::vector<double> build_interpolated_values(const std::string& coef_type, 
                                                          const RelocalisationFunction& reloc_f1, 
                                                          const RelocalisationFunction& reloc_f2, 
                                                          std::vector<double>& merged_angles, double ph_ratio) const;


            /// @brief Set periodic coefficients for angles in [90, 180] based on [0, 90] values
            void set_periodic_conditions_up();
            /// @brief Set periodic coefficients for angles in [180, 360] based on [0, 180] values
            void set_periodic_conditions_down();

        protected :
            /// @brief Return the relocalisation coefficient associated to a given angle
            /// @param angle given angle
            /// @param type type of relocalisation coefficient "a_phi", "b_phi" or "c_phi" 
            /// @return Value of relocalisation coefficient
            double get_relocalization_coef(const double& angle, const std::string& type) const;

            /// @brief Values of relocalisation coefficients for a list of angles. The possible
            /// keys are: "a_phi", "b_phi" or "c_phi" 
            std::unordered_map<std::string, amath::Table> _coefficients_;
            /// @brief type of lattice
            std::string _type_ = "";

        public:

            RelocalisationFunction() = default;
            RelocalisationFunction(const PlateCoefficients& plate_coef);
            virtual ~RelocalisationFunction() = default;

            /// @brief Get the type of relocalisation coefficients
            std::string get_type() const { return _type_; }
            /// @brief Set the type of relocalisation coefficients
            /// @param a_type type of relocalisation coefficients
            void set_type(const std::string& a_type) { _type_ = a_type; }
            /// @brief Get the number of relocalisation coefficients stored in the object
            /// @return number of relocalisation coefficients
            std::size_t size() { return _coefficients_.size(); }
            /// @brief Get the list of angles for which relocalisation coefficients are stored
            /// @return list of angles
            std::vector<double> get_angles() const { return _coefficients_.begin()->second.get_xvalues(); }

            /// @brief Build Relocalisation function for two given plate coefficients
            /// @param ph Expected \f$ P/h \f$ ratio
            /// @param coef1 first plate coefficients
            /// @param coef2 second plate coefficients
            void build_relocalisation_function(const double &ph, const PlateCoefficients &coef1, const PlateCoefficients &coef2);

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
            /// @brief Get value of relocalisation coefficient a for a given angle
            /// @param type type of relocalisation coefficient: "a_phi", "b_phi" or "c_phi"
            /// @param angle given angle
            /// @return Value of relocalisation coefficient
            double get_coef(const std::string& type, const double& angle) const;

            /// @brief Clone the current object
            /// @return shared pointer to the cloned object
            std::shared_ptr<RelocalisationFunction> clone() const;
            /// @brief Set periodic coefficients if necesssary
            void set_periodic_conditions();
    };
}