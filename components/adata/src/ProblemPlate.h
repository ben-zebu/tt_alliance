#pragma once

#include "Commands.h"
#include "Environment.h"
#include "FileReader.h"

namespace adata {

    class RelocalizationCoefficients;

    struct plate_angle {
        /// @brief angle step
        double delta = 0.;
        /// @brief maximum angle value
        double max = 180.;
        /// @brief minimum angle value
        double min = 0.;
        /// @brief list of angles values
        std::vector<double> values;

        /// @brief initialize the object with the given values
        /// @param delta angle step
        /// @param max maximum angle value
        /// @param min minimum angle value (default 0.)
        void init(double delta, double max, double min = 0.);
    };

    class ProblemPlate {
        private :
            /// @brief control the coherence between \f$ \phi \f$ and remarkable angles
            /// @param filecontext file content
            void verify_phi(const std::string& filecontext) const;

            /// @brief Initialize the object dedicated to user's relocalizaztion coefficients with the values read
            /// from the input file
            /// @param command values read from the input file
            void init_user_coefficients(const std::shared_ptr<abase::BaseCommand>& command);

        public:
            /// @brief store P/h ratio for primary and secondary side
            std::pair<double, double> Ph = {0., 0.};
            /// @brief type of plate VER : square or triangular
            std::string type = "";
            /// @brief sub case of the plate used for relocalization coefficients
            std::string sub_case = "";
            
            /// @brief boolean flag to check if the analysis is 2D
            bool if_2D = false;
            /// @brief boolean flag to check if the analysis is only a single angle
            bool if_single_angle = false;
            /// @brief boolean flag to check if the 3 Sm analysis must be performed
            bool if_3Sm = true;
            /// @brief category of the analysis
            std::size_t category = 0;

            /// @brief \f$ \theta \f$ angle parameters around the global Z axis (only for 2D analysis)
            plate_angle theta;
            /// @brief \f$ \phi \f$ angle parameters around the drilling hole axis
            plate_angle phi;

            /// @brief User's relocalisation coefficients
            std::shared_ptr<RelocalizationCoefficients> user_coefficients = nullptr;

            ProblemPlate() = default;
            virtual ~ProblemPlate() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            /// @param category analysis category
            void init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t category);
            /// @brief Verify the coherence of object values
            /// @param filecontext file content
            void verify(const std::string& filecontext) const;

            /// @brief Return the \f$ \phi \f$ angles used for 3 Sm analysis or primary stress analysis
            /// @return 3 Sm angles
            std::vector<double> get_3Sm_angles() const;
    };
}