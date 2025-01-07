#pragma once

#include <unordered_map>

#include "DataCollector.h"
#include "Environment.h"
#include "PlateCoefficients.h"

namespace adata {

    using namespace adata::parts;

    /// @brief Class to store a collection of plate coefficients
    class PlateCoefficientsCollector : public abase::DataCollector {
        private:
            /// @brief Add a plate coefficient object to the collection
            /// @param coef plate coefficient
            /// @param filecontext file context
            void add_coefficient(std::shared_ptr<PlateCoefficients> coef, const std::string& filecontext);

        protected:
            /// @brief Convert read data from input file into data objects
            /// @param command read data from the input file
            /// @param filecontext file context
            virtual void set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext);

            /// @brief Collection of plate coefficients
            std::vector<std::shared_ptr<PlateCoefficients>> coefficients_data;

        public:            
            PlateCoefficientsCollector() = default;
            ~PlateCoefficientsCollector() = default;

            /// @brief Get the plate coefficient associated to a given identification
            /// @param coefficient_id identification of the plate coefficient
            /// @param ph value of the P/h parameter
            /// @return shared pointer to the plate coefficient
            std::shared_ptr<PlateCoefficients> get_coefficient(const std::string& function_id, const double ph) const;
            /// @brief Return the lower and upper bounds of a given P/h parameter for a given plate coefficient
            /// @param function_id identification of the plate coefficient
            /// @param ph value of the P/h parameter
            /// @return pair of lower and upper bounds of P/h parameter
            std::pair<double, double> get_ph_range(const std::string& function_id, const double ph) const;

            /// @brief Clear the collection of plate coefficients
            void clear() { for (auto& data : coefficients_data) data->clear(); }
    };
}