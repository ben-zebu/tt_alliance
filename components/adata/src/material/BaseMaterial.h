#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

#include "FatigueLaw.h"

namespace adata {

    /// @brief Base class for material properties
    class BaseMaterial {
        protected:

        public:
            /// @brief Identification of the material
            std::string material_id = "";
            /// @brief Identification of fatigue law associated to the material
            std::string fatigue_law_id = "";
            /// @brief Type of the material
            std::string steel_type = "";
            /// @brief Type of mean stress correction
            std::string correction = "";
            /// @brief Effective factor used during the mean stress correction
            double Kf = 1.;
            /// @brief M Coefficient used during elastic-plastic correction factor
            double m = 0.;
            /// @brief N Coefficient used during elastic-plastic correction factor
            double n = 1.;

            BaseMaterial() = default;
            virtual ~BaseMaterial() = default;
            BaseMaterial& operator=(const BaseMaterial& material);

            /// @brief Command used to read the input file and initialize the object
            /// @param command command read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Verify if the object is correctly initialized
            /// @param filecontext file input context
            virtual void verify(const std::string& filecontext) const;
    };



}