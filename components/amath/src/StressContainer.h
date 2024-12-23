#pragma once

#include <string>
#include <vector>

#include "Combination.h"
#include "Stress.h"

namespace amath {

    /// @brief Utilty structure used to extract details results for a stress intensity 
    struct StressIntensity {
        /// \brief The stress intensity.
        double intensity = 0.;
        /// \brief The load number associated with the stress intensity.
        std::size_t load = 0;
        /// \brief The torsor combination number associated with the stress intensity.
        std::size_t torsor = 0;

        // Function to set values
        void set(double intensity_val, std::size_t load_val, std::size_t torsor_val) {
            intensity = intensity_val;
            load = load_val;
            torsor = torsor_val;
        }
    };

    /// @brief Utilty structure used to extract details results for a stress range 
    struct StressRange {
        /// \brief The stresss range.
        double range = 0.;
        /// \brief The mean stress.
        double mean = 0.;
        /// \brief The maximum ratio between stress range and a coefficient.
        double ratio = 0.;
        /// \brief The load numbers associated with the stress range and the mean stress.
        combi_ranks loads = {0, 0};
        /// \brief The torsor combination rank associated with the stress range and the mean stress.
        std::size_t torsor = 0;
        /// \brief Temperatures associated to loads given the stress range
        std::pair<double, double> temperatures = {0., 0.};

        // Function to set values
        void set(double range_val, double mean_val, double ratio_val, combi_ranks loads_val, std::size_t torsor_val) {
            range = range_val;
            mean = mean_val;
            ratio = ratio_val;
            loads = loads_val;
            torsor = torsor_val;
        }
    };


    /// @class StressContainer StressContainer.h 
    /// @brief Use to store results for a stress intensity or a stress range intensity.
    class StressContainer {
        protected :
            /// \brief The stresss range.
            double _intensity_ = 0.;
            /// \brief The mean stress.
            double _mean_ = 0.;
            /// \brief The maximum ratio between stress range and a coefficient.
            double _ratio_ = 0.;
            /// \brief The load numbers associated with the stress range and the mean stress.
            combi_ranks _loads_ = {0, 0};
            /// \brief The torsor combination numbers associated with the stress range and the mean stress.
            std::size_t _torsor_ = 0;
            /// \brief Temperatures associated to loads given the stress range
            std::pair<double, double> _temperatures_ = {0., 0.};

        private :

        public :

            /// @brief default empty constructor
            StressContainer() = default;
            /// @brief constructor by copy
            /// @param other an other StressContainer object
            StressContainer(const StressContainer& other);
            /// @brief Copy constructor 
            /// @param other an other StressContainer object
            StressContainer& operator=(const StressContainer& other);

            /// @brief Equal operator based on `_ratio_`.
            bool operator==(const StressContainer& other);
            /// @brief Difference operator based on `_ratio_`.
            bool operator!=(const StressContainer& other);
            /// @brief Less or equal operator based on `_ratio_`.
            bool operator<=(const StressContainer& other);
            /// @brief Upper or equal operator based on `_ratio_`.
            bool operator>=(const StressContainer& other);
            /// @brief Less operator based on `_ratio_`.
            bool operator<(const StressContainer& other);
            /// @brief Upper operator based on `_ratio_`.
            bool operator>(const StressContainer& other);

            /// @brief return the stored ratio
            double get_ratio() const { return _ratio_; }

            /// @brief Set a stress intensity
            /// @param intensity equivalent stress intensity
            /// @param load load rank associated to the stress intensity
            /// @param torsor torsor combination rank associated to the stress intensity
            void set_intensity(const double& intensity, const std::size_t load, const std::size_t torsor);

            /// @brief Set a stress range intensity
            /// @param range vector composed of equivalent stress range intensity, ratio with a coefficient and the 
            /// mean stress
            /// @param load loads' ranks associated to the stress intensity
            /// @param torsor torsor combination rank associated to the stress intensity
            void set_range(const std::vector<double>& range, const combi_ranks& load, const std::size_t torsor);

            /// @brief Set the temperatures associated to the stress range
            /// @param T1 temperature associated to the first load
            /// @param T2 temperature associated to the second load
            void set_temperatures(const double& T1, const double& T2);
            /// @brief Set the mean stress associated to the stress range
            /// @param mean value of mean stress
            void set_mean_stress(const double& mean) { _mean_ = mean; }

            /// @brief Update internals parameters for maximal stress intensity
            /// @param other a stess range
            void store_max(const StressContainer& other);

            /// @brief Return a structure with detailed results for stress intensity
            /// @return detailed stress intensity
            StressIntensity get_intensity() const;
            /// @brief Return a structure with detailed results for stress range
            /// @return detailed stress range
            StressRange get_range() const;

    };
}