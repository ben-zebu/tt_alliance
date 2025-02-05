#pragma once

#include <array>
#include <vector>

namespace amath {

    /// \brief Pair of ranks
    using combi_ranks = std::pair<std::size_t, std::size_t>;

    /*!
     * \brief Class used to manage all possible ranks' combinations given by a vector of ranks.
     *
     * \details This class is used to manage the ranks' combination identification given by a vector of ranks. The naive 
     * represention of all combinations is a square matrix storage with a size equal to vector of ranks. Each element
     * of the matrix represents a combination of ranks. The associated ranks to a combination is identified by the 
     * row and column indices. Several cases are possible:
     *  - The total square matrix if the expected results between (i, j) ranks is different than (j, i) ranks.
     *  - the triangular matrix if the expected results between (i, j) ranks is equal to (j, i) ranks.
     *  - The superior triangular matrix if the expected results between (i, j) ranks is equal to (j, i) ranks and the
     *    the combination (i, i) is excluded.
     */
    class Combination {
        protected:
            /// \brief Vector of ranks.
            std::vector<std::size_t> ranks;
            /// \brief  The number of possible combinations
            std::size_t cached_size;        
        private:
            /// \brief Function used to order ranks and suppress duplicates.
            void sort_ranks();
            /// \brief  Store the number of combinations
            virtual void set_cached_size() = 0;

        public:
            /// \brief Constructor.
            /// \param ranks Vector of ranks.
            Combination(const std::vector<std::size_t>& ranks);
            /// \brief Constructor based on 2 vector of ranks.
            /// \param rks_1 First vector of ranks.
            /// \param rks_2 Second vector of ranks.
            Combination(const std::vector<std::size_t>& rks_1, const std::vector<std::size_t>& rks_2);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            Combination(const Combination& combination);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            Combination& operator=(const Combination& combination);

            /// \brief Return the number of combinations.
            /// \return Number of combinations.
            virtual std::size_t size() const { return cached_size; };

            /// \brief Return the combination associated to the given indices.
            /// \param row Row index.
            /// \param column Column index.
            /// \return Combination associated to the given indices.
            virtual std::size_t operator()(const std::size_t& row, const std::size_t& column) const = 0;

            /// \brief Return the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual combi_ranks get_ranks(const std::size_t& combination) const = 0;
            /// \brief Return by pointer the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual void ranks_by_ptr(const std::size_t& combination, combi_ranks& ranks) const = 0;
    };

    /*!
     * \brief Class used to manage all possible ranks' combinations given by a vector of ranks. These ranks can be 
     * provided from two separate vectors.
     *
     * \details All combination are given by a NxM matrix representation. The number of rows is equal to the size of 
     * the first vector. The number of columns is equal to the size of the second vector. 
     * The representation is the following matrix:
     *     \f{eqnarray*}{  
     *         \left [ \begin{array}{cccc}
     *             (0,0)   & (0,1)   &  ...    & (0,m-1) \\
     *             (1,0)   & (1,1)   &  ...    & (1,m-1) \\
     *             (2,0)   & (2,1)   & (2,2)   & (2,m-1) \\
     *             (n-1,0) & (n-1,1) & (n-1,2) & (n-1,m-1) 
     *          \end{array} \right ] 
     *     \f} 
     */
    class RectangularCombination : public Combination {
        private:
            /// \brief number of rows
            std::size_t nb_rows = 0;
            /// \brief number of columns
            std::size_t nb_columns = 0;

            /// \brief  Store the number of combinations
            virtual void set_cached_size() { cached_size = nb_rows * nb_columns; };

        public:
            /// \brief Constructor.
            /// \param ranks Vector of ranks.
            RectangularCombination(const std::vector<std::size_t>& ranks);
            /// \brief Constructor based on 2 vector of ranks.
            /// \param rks_1 First vector of ranks.
            /// \param rks_2 Second vector of ranks.
            RectangularCombination(const std::vector<std::size_t>& rks_1, const std::vector<std::size_t>& rks_2);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            RectangularCombination(const RectangularCombination& combination);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            RectangularCombination& operator=(const RectangularCombination& combination);

            /// \brief Return the combination associated to the given indices
            /// \param row Row index.
            /// \param column Column index.
            /// \return Combination associated to the given indices.
            virtual std::size_t operator()(const std::size_t& row, const std::size_t& column) const;

            /// \brief Return the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual combi_ranks get_ranks(const std::size_t& combination) const;
            /// \brief Return by pointer the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            void ranks_by_ptr(const std::size_t& combination, combi_ranks& ranks) const;
    };

    /*!
     *  \brief Class used to manage all possible ranks' combinations given by a vector of ranks.
     * 
     *  \details All combination are given by a triangular matrix representation with a size equal to vector
     *  of ranks. The allowed cases are:
     *   - without diagonnal terms and the following matrix:
     *      \f{eqnarray*}{  
     *          \left [ \begin{array}{cccc}
     *               na   & (0,1) &  ...  & (0,n-1) \\
     *               na   &  na   &  ...  & (1,n-1) \\
     *               na   &  na   &  na   & (2,n-1) \\
     *               na   &  na   &  na   &  na 
     *          \end{array} \right ] 
     *      \f}
     *   - with diagonnal terms and the following matrix:
     *      \f{eqnarray*}{  
     *          \left [ \begin{array}{cccc}
     *              (0,0) & (0,1) &  ...  & (0,n-1) \\
     *               na   & (1,1) &  ...  & (1,n-1) \\
     *               na   &  na   & (2,2) & (2,n-1) \\
     *               na   &  na   &  na   & (n-1,n-1) 
     *          \end{array} \right ] 
     *      \f}
     */
    class TriangularCombination : public Combination {
        private:
            /// \brief  Store the number of combinations
            void set_cached_size();

            /// \brief Determine the number of combinations stored in the first \f$ p \f$ lines.
            ///
            /// \details Determine the number of combinations stored in the first \f$ p \f$ lines based on the 
            /// following formula:
            /// \f[ S(p,n) = \sum_{i=0}^{p} d(n) - i = \sum_{j=d(n)-p}^{d(n)} j = \frac{1}{2} (p+1)(2d(n)-p) \f]
            /// with :
            ///  - \f$ d(n) = n \f$ if diagonal is set,
            ///  - \f$ d(n) = n - 1\f$ if diagonal is not set.
            /// The \f$ d(n) \f$ result is given by function \ref diag_shift()
            /// \param p Number of lines.
            /// \param n Number of ranks.
            /// \return First combination represented on the p line.
            constexpr std::size_t combination_for_line(const std::size_t& p, const std::size_t n) const;
            /// \brief Determine the line associated to a combination.
            ///
            /// \details Return the \f$ p \f$ line associated to the combination \f$ c \f$ such as:
            /// \f$ S(p-1,n) \leq c \leq S(p,n) \f$ where \f$ S(p,n) \f$ gives the number of combination stored in the
            /// \f$ p \f$ lines (see \ref combination_for_line()).
            /// The \f$ p \f$ line is the upper integer associated to the lower root of the following polynom:
            ///     \f[ p^2 + b.p + 2(c+1) \f]
            /// with \f$ b = 1 +2 d(n) \f$ and \f$ d(n) \f$ the result is given by function \ref diag_shift()
            /// \param c combination number.
            /// \param n Number of ranks.
            /// \return line number associated to the combination c.
            constexpr std::size_t line_for_combination(const std::size_t& c, const std::size_t n) const;

            /// \brief Booleen parameter used to check if elements are stored on the diagonal of the matrix representation.
            bool _on_diag_ = false;
            /// \brief Return the number of elements for the first line in the matrix representation.
            constexpr std::size_t diag_shift(const std::size_t& n) const { return _on_diag_ ? n : n - 1; }

        public:
            /// \brief Constructor.
            /// \param ranks Vector of ranks.
            /// \param on_diag boolean for use or not of the diagonal
            TriangularCombination(const std::vector<std::size_t>& ranks, bool on_diag = false);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            /// \param on_diag boolean for use or not of the diagonal
            TriangularCombination(const TriangularCombination& combination, bool on_diag = false);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            TriangularCombination& operator=(const TriangularCombination& combination);
            /// \brief Set if combinations with same indices for row and column are possible
            void set_type(bool on_diag);
            /// \brief Return if combinations with same indices for row and column are possible
            bool get_type() const { return _on_diag_; }

            /// \brief Return the combination associated to the given indices.
            /// \param row Row index.
            /// \param column Column index.
            /// \return Combination associated to the given indices.
            virtual std::size_t operator()(const std::size_t& row, const std::size_t& column) const;

            /// \brief Return the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual combi_ranks get_ranks(const std::size_t& combination) const;
            /// \brief Return by pointer the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            void ranks_by_ptr(const std::size_t& combination, combi_ranks& ranks) const;
    };

};