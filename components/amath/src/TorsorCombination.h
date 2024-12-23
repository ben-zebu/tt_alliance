#pragma once

#include <string>
#include <vector>

#include "Combination.h"

namespace amath {

    /// @brief Value used to check if 2 coefficients are equal
    static constexpr double COEFFICIENT_TOLERANCE = 1e-6;

    /// @brief Class used to manage the torsor's combinations. All possible torsor combination are represented by 
    /// a binary tree. The following representation is given for 4 torsors, the second one is constant. 
    /// @verbatim
    ///                                   tree_rk
    ///                           / Max      0
    ///                     / Max   
    ///                    /     \ min       1
    ///       / Max --- cst 
    ///      /             \     / Max       2
    ///     /               \ min
    ///    /                     \ min       3
    ///   *
    ///    \                     / Max       4
    ///     \               / Max
    ///      \             /     \ min       5
    ///       \ min --- cst
    ///                    \     / Max       6
    ///                     \ min 
    ///                          \ min       7
    /// @endverbatim
    /// The number of torsor combination is equal to \f$ 2^t \f$ where \f$ t \f$ is the number of torsor with different.
    /// coefficients.
    ///
    /// For a given torsor combination, the choice between the maximum and the minimun value is based on the parity of
    /// each explorer branch. For the 5th combination of the example, the retained coefficients are:
    ///  - 4th torsor associated to \f$ tree\_rk = 5 \rightarrow \f$ minimal coefficient
    ///  - 3rd torsor associated to \f$ tree\_rk = 5//2 = 2 \rightarrow \f$ maximal coefficient
    ///  - 2nd torsor associated to \f$ tree\_rk = 2 \rightarrow \f$ constant coefficient
    ///  - 1st torsor associated to \f$ tree\_rk = 2//2 = 1 \rightarrow \f$ minimal coefficient
    class TorsorCombination {

        private:
            /// \brief internal status
            bool _is_activate_ = false;
            /// @brief internal size parameter
            std::size_t cached_size = 0;

            /// @brief Check data integrity.
            void _check_() const;

            /// @brief check if torsor coefficients for 2 states are constant
            /// @param states pair of state
            /// @param torsor_rk torsor rank
            bool constant_coefficients(const combi_ranks& states, std::size_t torsor_rk) const;

            /// @brief Return the difference or the sum of coefficients for a pair of states and a torsor's combination. 
            /// @param states pair of states
            /// @param tcomb torsor's combinations
            /// @param cl coefficients used for linear combinations of torsors's coefficient. 
            /// The couple \f$ (1,1) $\f represents a sum and the couple \f$ (1,-1) $\f represents a difference and 
            /// @param[out] coefficients list of coefficients
            void get_combi_coef(combi_ranks states, std::size_t tcomb, const std::pair<double,double>& cl, std::vector<double>& coefficients) const;
            
        protected:
            /// @brief vector of active torsors
            std::vector<bool> active_torsors;
            /// @brief pointer to the maximum coefficients for each state and each torsor (\ref StressStates)
            std::vector<std::vector<double>>* ptr_coef_max = nullptr;
            /// @brief pointer to the minimum coefficients for each state and each torsor (\ref StressStates)
            std::vector<std::vector<double>>* ptr_coef_min = nullptr;

        public:
            /// @brief default construtor
            TorsorCombination() = default;
            /// @brief constructor by copy
            /// @param other the object to copy
            TorsorCombination(const TorsorCombination& other);
            /// @bried Reset internal parameters.
            void reset();
            /// @brief Return the activation status
            /// @return current status 
            bool is_activate() const { return _is_activate_; }

            /// @brief Create the link by pointers to the torsor coefficients. This function must be called before
            /// `set_active_torsors`.
            /// @param CoefMax vector of maximum coefficients
            /// @param CoefMin vector of maximum coefficients
            void set_ptr_coefficients(std::vector<std::vector<double>>& CoefMax, std::vector<std::vector<double>>& CoefMin);
            /// @brief Set active torsors. This function must be called after the function `set_ptr_coefficients`.
            /// @param torsors status for each torsor
            void set_active_torsors(const std::vector<bool>& torsors);
            /// @brief Return the list of coefficients for a state and a torsor's combination 
            /// @param state current state
            /// @param tcomb torsor's combination
            /// @param[out] coefficients list of coefficients
            void get_coef(std::size_t state, std::size_t tcomb, std::vector<double>& coefficients) const;
            /// @brief Return the difference of coefficients for a pair of states and a torsor's combination. 
            /// @param states pair of states
            /// @param tcomb torsor's combinations
            /// @param[out] coefficients list of coefficients
            void get_diff_coef(combi_ranks states, std::size_t tcomb, std::vector<double>& coefficients) const;
            /// @brief Return the sum of coefficients for a pair of states and a torsor's combination. 
            /// @param states pair of states
            /// @param tcomb torsor's combinations
            /// @param[out] coefficients list of coefficients
            void get_sum_coef(combi_ranks states, std::size_t tcomb, std::vector<double>& coefficients) const;

            /// @brief Return the number of torsor's combinations for a state
            /// @param state current state
            std::size_t nb_combinaisons(std::size_t state) const;
            /// @brief Return the number of torsor's combinations for a pair of states
            /// @param states pair of states
            std::size_t nb_combinaisons(const combi_ranks& states) const;

    };

}            