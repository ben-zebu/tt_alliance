#pragma once

#include <string>
#include <vector>

#include "Coefficient.h"
#include "Combination.h"
#include "Stress.h"

namespace amath {

    /// \brief Represents a stress intensity.
    struct StressIntensity {
        /// \brief The stress intensity.
        double Si = 0.;
        /// \brief The load number associated with the stress intensity.
        std::size_t load = 0;
        /// \brief The torsor combination number associated with the stress intensity.
        std::size_t torsor = 0;
    };

    /// \brief Represents a stress range.
    struct StressRange {
        /// \brief The stresss range.
        double Sr = 0.;
        /// \brief The mean stress.
        double Smean = 0.;
        /// \brief The maximum ratio between stress range and a coefficient.
        double ratio = 0.;
        /// \brief The load numbers associated with the stress range and the mean stress.
        std::size_t loads[2] = {0, 0};
        /// \brief The torsor combination numbers associated with the stress range and the mean stress.
        std::size_t torsors[2] = {0, 0};
    };

    /// \brief Represents a collection of stress states.
    ///
    /// This class provides methods to add stress states, calculate stress intensities, and perform various stress 
    /// calculations.
    class StressStates {

        private:
            /// \brief Check the data integrety of the stress states.
            void _check_();
        public:
            /// \brief Maximum equivalent stress determination
            /// \param[out] Sr_max The maximum stress range.
            /// \param stress The stress state.
            /// \param coef The coefficient used to compute the stress range ratio.
            /// \param loads The load numbers associated with the stress range.
            void _maximum_equivalent_stress_(StressRange& Sr_max, const Stress& stress, const double coef, const combi_ranks& loads);

            
        protected:
            /// \brief A vector of primary stress states. If secondary stresses are not provided, the primary stresses
            /// stores the cumul of primary and secondary stress.
            std::vector<Stress> PrimaryStresses;
            /// \brief A vector of secondary stress states.
            std::vector<Stress> SecondaryStresses;
            /// \brief A vector of temperature states.
            std::vector<double> Temperatures;

            /// \brief A vector of stress states given by external torsors. 
            std::vector<Stress> Torsors;
            /// \brief A vector of maximal coefficients associated to each torsor.
            ///
            /// Coefficients are defined for each possible state. For each state, the number of coefficients is equal 
            /// to the number of torsors. The following representation with `state_id` as the state  index and 
            /// `torsor_id` as the torsor index is used:
            ///   
            ///   \code CoefficientsMax[state_id][torsor_id] \endcode
            /// 
            std::vector<std::vector<double>> CoefficientsMax;
            /// \brief A vector of minimal coefficients associated to each torsor.
            ///
            /// Coefficients are defined for each possible state. For each state, the number of coefficients is equal 
            /// to the number of torsors. The following representation with `state_id` as the state  index and 
            /// `torsor_id` as the torsor index is used:
            ///   
            ///   \code CoefficientsMax[state_id][torsor_id] \endcode
            /// 
            std::vector<std::vector<double>> CoefficientsMin;

            /// \brief Method used to calculate the equivalent stress.
            std::string equivalent_stress_method = "tresca";

        public:

            /// \brief Reset all stress states.
            void reset();
            /// \brief Check the data integrety of the stress states.
            void check_integrity() { _check_(); };
            /// \brief Set the equivalent stress method.
            /// \param method The equivalent stress method.
            void set_equivalent_stress_method(const std::string& method);
            /// \brief Get the number of stress states.
            /// \return The number of stress states.
            size_t size() const { return PrimaryStresses.size(); };

            /// \brief Add a stress state.
            /// \param primary_stress The primary stress state to add.
            /// \param secondary_stress The secondary stress state to add.
            void add_stress(const Stress& primary_stress, const Stress& secondary_stress = Stress());
            /// \brief Add a temperature state.
            /// \param temperature The temperature state to add.
            void add_temperature(const double& temperature);
            /// \brief Add a torsor.
            /// \param torsor The torsor to add.
            void add_torsor(const Stress& torsor);
            /// \brief Add torsor coefficients for a given state.
            /// \param cmax The maximal coefficients to add.
            /// \param cmin The minimal coefficients to add.
            void add_torsor_coefficients(const std::vector<double>& cmax, const std::vector<double>& cmin);

            /// \brief Calculate the maximum stress intensity.
            /// \param states_id The states id used to calculate the stress intensity.
            /// \return The maximum stress intensity.
            StressIntensity stress_intensity(const std::vector<size_t>& states_id);

            /// \brief Calculate the maximum stress range.
            /// \param states_id The states id used to calculate the stress range.
            /// \return The maximum stress range.
            StressRange stress_range(const std::vector<size_t>& states_id);
            /// \brief Calculate the maximum ratio between stress range and a coefficient.
            /// \param states_id The states id used to calculate the stress range.
            /// \param coefficient The coefficient used to calculate the maximum ratio.
            /// \return The maximum stress range.
            StressRange stress_range_ratio(const std::vector<size_t>& states_id, const Coefficient& coefficient);

            /// \brief Calculate the maximum stress range.
            /// \param explorer combinations' explorer
            /// \return The maximum stress range.
            StressRange stress_range(const Combination& explorer);
            /// \brief Calculate the maximum stress range.
            /// \param explorer combinations' explorer
            /// \return The maximum stress range.
            StressRange stress_range_ratio(const Combination& explorer, const Coefficient& coefficient);

    };

}            