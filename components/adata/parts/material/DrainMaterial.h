#pragma once

#include "BaseMaterial.h"

namespace adata::parts {

    class DrainMaterial : public BaseMaterial {
        protected:

        public:
            /// @brief Stress intensities depending on the angle of the drain hole
            amath::Table stress_intensities;
            /// @brief Stress ratio used to build the fatigue law associated to the drain hole
            double stress_ratio = 1.;
            /// @brief Maximum stress allowed for the drain hole fatigue law
            double stress_limit;

            DrainMaterial() = default;
            virtual ~DrainMaterial() = default;

            DrainMaterial& operator=(const DrainMaterial& material);

            /// @brief Command used to read the input file and initialize the object
            /// @param command command read from the input file
            virtual void init(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Verify if the object is correctly initialized
            /// @param filecontext file input context
            virtual void verify(const std::string& filecontext) const;
    };

}