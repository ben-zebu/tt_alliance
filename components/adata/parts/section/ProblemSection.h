#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

#include "ProblemSectionCoefficient.h"

namespace adata::parts {

    /// @brief Value used as a flag to check if a std::size_t value has been defined or not
    static constexpr std::size_t UNDEFINED_SIZE_T = std::numeric_limits<std::size_t>::max();

    struct StressCoefficientContainer {
        PKCoefficient pk;
        KFCoefficient kf;
        KMCoefficient km;
        KHCoefficient kh;
        OvalCoefficient oval;
        MBCoefficient mb;
        PMBCoefficient pmb;
        BTCoefficient bt;

        // @brief Initialize the object with the values read from the input file
        /// @param command command read from the input file
        void init(const std::shared_ptr<abase::BaseCommand>& command);
        /// @brief Verify the coherence of each coefficient
        void verify(const std::string& filecontext) const;
    };

    class ProblemSection {
        private :
            /// @brief Set the default name of the object
            /// @param id rank associated to the section in the input file
            void default_name(std::size_t id) { internal_name = "SECTION_" + std::to_string(id); };

            /// @brief Set material parameters such as EE, SM, SU, etc. from the input command
            /// @param command input command
            void set_material_parameters(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Set the material definitions from the input command 
            /// @param command input command
            void set_material_definition(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Set special parameters from the input command
            /// @param command input command
            void set_special_parameters(const std::shared_ptr<abase::BaseCommand>& command);

        protected:
            /// @brief Category of the problem
            std::size_t _category_ = UNDEFINED_SIZE_T;

        public :
            /// @brief Rank of th section in FEM Interface (default value is the maximum value of std::size_t)
            std::size_t fem_rank = UNDEFINED_SIZE_T;
            /// @brief Name of the section
            std::string name = "";
            /// @brief internal name of the section (based on its rank from the input file)
            std::string internal_name = "";

            /// @brief material id associate to the section. A material name is associated for each skin of the section
            std::unordered_map<std::string, std::string> material_id;
            /// @brief fatigue law id associate to the section
            std::string fatigue_law_id = "";

            /// @brief Type of temperature dependence for \f$ \frac{E_c}{E} \f$ calculation. The expected values are:
            ///  - "none" : no temperature dependence
            ///  - "mean" : mean value of 2 timesteps
            ///  - "min" : minimum value of 2 timesteps
            ///  - "max" : maximum value of 2 timesteps
            std::string EcE_type = "mean";

            /// @brief Material input values used to override standard definition. The different
            /// keys are:
            ///  - "EE" : Young modulus \f$ E \f$
            ///  - "SM" : Admissible stress \f$ S_m \f$
            ///  - "SU" : Ultimate tensile stress \f$ S_u \f$
            ///  - "SCY" : Cyclic yield stress \f$ S_{cy} \f$
            ///  - "SY" : Conventionnal yield stress \f$ S_y \f$
            ///  - "SYG" : Conventionnal yield stress for a large number of cycles \f$ S_{yg} \f$
            /// The dedicated material will be based on the standard definition then the input values will be used to
            /// overide the standard definition.
            std::unordered_map<std::string, std::pair<std::string, double>> material_parameters_input;

            /// @brief Parameter used to define if the section is in an drilled part or not
            bool is_drilled = false;
            /// @brief Parameter used to activate the Neuber elastoplastic correction
            bool neuber_correction = false;
            /// @brief Parameter used to activate the Mises criterion for equivalent stress
            bool mises_criterion = false;

            /// @brief Stress coefficients associate to the section
            StressCoefficientContainer stress_coefficients;

            ProblemSection() = default;
            virtual ~ProblemSection() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command command read from the input file
            /// @param id rank associated to the section
            /// @param category category of the problem (used for futher verification)
            void init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t id, std::size_t category);
            /// @brief Verify the coherence of the material definition
            void verify(const std::string& filecontext) const;

    };
}
