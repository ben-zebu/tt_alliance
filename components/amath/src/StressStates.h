#pragma once

#include <string>
#include <vector>

#include "Coefficient.h"
//#include "Combination.h"
#include "Stress.h"
#include "StressContainer.h"
#include "TorsorCombination.h"

namespace amath {    

    /// \brief Represents a collection of stress states.
    ///
    /// This class provides methods to add stress states, calculate stress intensities, and perform various stress 
    /// calculations.
    class StressStates {

        private:
            /// \brief Check the data integrety of the stress states.
            void _check_();

            /// \brief Maximum equivalent stress determination
            /// \param[out] Sc_max The maximum stress range or stress intensity.
            /// \param stress The stress state.
            /// \param coef The coefficient used to compute the stress range ratio.
            /// \param loads The load numbers associated with the stress range.
            void _maximum_equivalent_stress_(StressContainer& Sr_max, const Stress& stress, const double coef, const combi_ranks& loads);
            /// @brief Return the maximum interpoletd coefficient for 2 states. Temperature of each state is used to
            /// determine the interpolated value
            /// @param coefficient coefficient used for interpolation
            /// @param ranks states ranks
            /// @return maximum interpoletd coefficient
            double get_interpolated_coeffient(const Coefficient& coefficient, const combi_ranks& ranks) const;
            
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

            /// \brief manage the torsor combination
            TorsorCombination torsors_manager;

        public:
            /// @brief destructor method
            ~StressStates();
            /// \brief Reset all stress states.
            void reset();
            /// \brief Check the data integrety of the stress states.
            void check_integrity() { _check_(); };
            /// \brief Set the equivalent stress method.
            /// \param method The equivalent stress method.
            void set_equivalent_stress_method(const std::string& method);
            /// \brief Get the number of stress states.
            /// \return The number of stress states.
            std::size_t size() const { return PrimaryStresses.size(); }
            /// \brief Get the number of torsors.
            /// \return The number of torsors.
            std::size_t nb_torsors() const { return Torsors.size(); }

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
            /// @brief Set active torsors in the combination manager
            /// @param active_torsors status list for each torsor
            void set_active_torsors(const std::vector<bool>& active_torsors);

            /// \brief Calculate the maximum stress intensity.
            /// \param states_id The states id used to calculate the stress intensity.
            /// \return The maximum stress intensity.
            StressContainer stress_intensity(const std::vector<size_t>& states_id);

            /// \brief Calculate the maximum stress range.
            /// \param explorer combinations' explorer
            /// \return The maximum stress range.
            StressContainer stress_range(const Combination& explorer);
            /// \brief Calculate the maximum stress range.
            /// \param explorer combinations' explorer
            /// \return The maximum stress range.
            StressContainer stress_range_ratio(const Combination& explorer, const Coefficient& coefficient);

    };

}            