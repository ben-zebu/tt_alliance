#pragma once

#include "Environment.h"

#include "BaseMaterialCollector.h"
#include "FatigueLawCollector.h"
#include "PlateCoefficientsCollector.h"

namespace adata {

    class Collections {
        private:
            void fill(abase::DataCollector& collector, const std::string& conf_files, const std::string& conf_commands);

        protected:
            BaseMaterialCollector materials;
            FatigueLawCollector laws;
            PlateCoefficientsCollector plate_coefficients;

        public:
            Collections();
            virtual ~Collections() = default;

            /// @brief Shortcut to BaseMaterialCollector::get_material function
            /// @param material_id material's identification
            /// @return shared pointer to the material
            std::shared_ptr<BaseMaterial> get_material(const std::string& material_id) const;

            /// @brief Shortcut to FatigueLawCollector::get_law function
            /// @param law_id identification of the fatigue law
            /// @param code construction code of the fatigue law
            /// @param edition construction code edition of the fatigue law
            /// @return shared pointer to the fatigue law
            std::shared_ptr<FatigueLaw> get_law(const std::string& law_id, 
                                                const std::string& code, const std::string& edition) const;

            /// @brief Shortcut to PlateCoefficientsCollector::get_coefficient function
            /// @param coefficient_id identification of the plate coefficient
            /// @param ph value of the P/h parameter
            /// @return shared pointer to the plate coefficient
            std::shared_ptr<PlateCoefficients> get_coefficient(const std::string& function_id, const double ph) const;
            /// @brief Shortcut to PlateCoefficientsCollector::get_ph_range function
            /// @param function_id identification of the plate coefficient
            /// @param ph value of the P/h parameter
            /// @return pair of lower and upper bounds of P/h parameter
            std::pair<double, double> get_ph_range(const std::string& function_id, const double ph) const;

    };


}