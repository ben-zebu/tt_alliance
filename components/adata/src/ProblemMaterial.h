#pragma once

#include "Environment.h"
#include "Coefficient.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata {

    class ProblemMaterial {
        private :
            /// @brief Set the default name of the object
            /// @param id rank associated to the table
            void default_name(std::size_t id) { name = "MATERIAL_" + std::to_string(id); };

            /// @brief Map used to convert the mean stress correction integer into an explicit string
            std::unordered_map<std::string, std::string> _correction_map = {
                {"none", "1"}, {"goodman", "2"}, {"gerber", "3"}, {"peterson", "4"}, {"haigh", "5"}
            };
            /// @brief Convert the mean stress correction integer or string into an explicit formated string
            /// @param int_correction mean stress correction integeror string
            /// @return mean stress correction string
            std::string _get_correction_name(const std::string& int_correction) const;

            void set_material_parameters(const std::shared_ptr<abase::BaseCommand>& command);

        public :
            /// @brief Name of the object
            std::string name = "";
            /// @brief Base material id associate to the object
            std::string base_material_id = "";
            /// @brief Type of temperature dependence for \f$ \frac{E_c}{E} \f$ calculation. The expected values are:
            ///  - "NONE" : no temperature dependence
            ///  - "MEAN" : mean value of 2 timesteps
            ///  - "MIN" : minimum value of 2 timesteps
            ///  - "MAX" : maximum value of 2 timesteps
            std::string EcE_type = "MEAN";

            std::unordered_map<std::string, amath::LinearCoefficient> material_parameters;
            /// @brief Map used to store the input values for all coefficients that defined the material. The different
            /// keys are:
            ///  - "EE" : Young modulus \f$ E \f$
            ///  - "SM" : Admissible stress \f$ S_m \f$
            ///  - "SU" : Ultimate tensile stress \f$ S_u \f$
            ///  - "SCY" : Cyclic yield stress \f$ S_{cy} \f$
            ///  - "SY" : Conventionnal yield stress \f$ S_y \f$
            ///  - "SYG" : Conventionnal yield stress for a large number of cycles \f$ S_{yg} \f$
            std::unordered_map<std::string, std::pair<std::string, double>> material_parameters_input;

            /// @brief Young modulus \f$ E \f$ for the material
            amath::LinearCoefficient E;
            /// @brief Admissible stress \f$ S_m \f$ for the material
            amath::LinearCoefficient Sm;
            /// @brief Ultimate tensile stress \f$ S_u \f$ for the material
            amath::LinearCoefficient Su;
            /// @brief Cyclic yield stress \f$ S_{cy} \f$ for the material used for mean stress correction with 
            /// experimental fatigue laws.
            amath::LinearCoefficient Scy;
            /// @brief Conventionnal yield stress \f$ S_y \f$ for the material (used for thermal ratchet). By default,
            /// it is equal to the ultimate tensile stress \f$ 1.5 S_m \f$.
            amath::LinearCoefficient Sy;
            /// @brief Conventionnal yield stress \f$ S_{yg} \f$ for a large number of cycles (used for thermal ratchet).
            amath::LinearCoefficient Syg;

            std::vector<std::string> Ke_table_name;

            /// @brief Reference Young modulus \f$ E_{c} \f$ for the material
            double Eref = 0.0;
            /// @brief Reference to base material for the choice of \f$ K_e \f$ calculation. Undefined values means
            /// that the base material method is used for the calculation.
            std::string typm = "";
            /// @brief Effective factor used during the mean stress correction
            double Kf = 1.;
            /// @brief Reference to the table used for user's fatigue law
            std::string user_law_table = "";
            /// @brief type of mean stress correction for the material. The expected values are:
            ///  - "none" or "1": no mean stress correction (default value)
            ///  - "goodman" or "2": Goodman mean stress correction
            ///  - "gerber" or "3": Gerber mean stress correction
            ///  - "perterson" or "4": Peterson mean stress correction
            ///  - "haigh" or "5": Haigh mean stress correction
            /// @note For compatibility reasons, the values "1", "2", "3", "4" and "5" are also accepted.
            std::string correction = "none";
            
            ProblemMaterial() = default;
            virtual ~ProblemMaterial() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command command read from the input file
            /// @param id rank associated to the material
            void init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t id);
            /// @brief Verify the coherence of the material definition
            void verify(const std::string& filecontext) const;
    };

}