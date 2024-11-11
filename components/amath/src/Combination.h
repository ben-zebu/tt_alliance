#pragma once

#include <vector>

namespace amath {

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
        
        private:
            /// \brief Function used to order ranks and suppress duplicates.
            void sort_ranks();

        public:
            /// \brief Constructor.
            /// \param ranks Vector of ranks.
            Combination(const std::vector<std::size_t>& ranks);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            Combination(const Combination& combination);
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            Combination& operator=(const Combination& combination);

            /// \brief Return the number of combinations.
            /// \return Number of combinations.
            virtual std::size_t size() const = 0;

            /// \brief Return the combination associated to the given indices.
            /// \param row Row index.
            /// \param column Column index.
            /// \return Combination associated to the given indices.
            virtual std::size_t operator()(const std::size_t& row, const std::size_t& column) const = 0;

            /// \brief Return the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual std::vector<std::size_t> get_ranks(const std::size_t& combination) const = 0;
            
    };

    /** \brief Class used to manage all possible ranks' combinations given by a vector of ranks.
     *
     * \details All combination are given by a matrix representation with a size equal to vector of ranks.
     * Its representation is the following matrix:
     *     \f{eqnarray*}{  
     *         \left [ \begin{array}{cccc}
     *             (0,0)   & (0,1)   &  ...    & (0,n-1) \\
     *             (1,0)   & (1,1)   &  ...    & (1,n-1) \\
     *             (2,0)   & (2,1)   & (2,2)   & (2,n-1) \\
     *             (n-1,0) & (n-1,1) & (n-1,2) & (n-1,n-1) 
     *          \end{array} \right ] 
     *     \f} 
     */
    class SquareCombination : public Combination {
       public:
            /// \brief Constructor.
            /// \param ranks Vector of ranks.
            SquareCombination(const std::vector<std::size_t>& ranks) : Combination(ranks) {};
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            SquareCombination(const SquareCombination& combination) : Combination(combination) {};
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            SquareCombination& operator=(const SquareCombination& combination);

            /// \brief Return the number of combinations.
            /// \return Number of combinations.
            virtual std::size_t size() const { return ranks.size() * ranks.size(); };

            /// \brief Return the combination associated to the given indices.
            /// \param row Row index.
            /// \param column Column index.
            /// \return Combination associated to the given indices.
            virtual std::size_t operator()(const std::size_t& row, const std::size_t& column) const;

            /// \brief Return the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual std::vector<std::size_t> get_ranks(const std::size_t& combination) const;        
    };

    /*!
     *  \brief Class used to manage all possible ranks' combinations given by a vector of ranks.
     * 
     *  \details All combination are given by a superior triangular matrix representation with a size equal to vector
     *  of ranks. Its representation is the following matrix:
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
            /// \brief Determine the first combination represented on the p line.
            ///
            /// \details Return the first combination represented on the p line and based on the following formula:
            /// \f[ S = \sum_{i=1}^{p} sz +1 - i = \sum_{j=sz+1-p}^{sz} j = \frac{1}{2} p (2sz+1-p) \f]
            /// with \f$ sz \f$ the size of the vector of ranks.
            /// \param p Number of lines.
            /// \return First combination represented on the p line.
            std::size_t combination_for_line(const std::size_t& p) const;
            /// \brief Determine the line associated to a combination.
            ///
            /// \details Return the \f$ p \f$ line associated to the combination \f$ c \f$ such as:
            /// \f$ S(p) \leq c \leq S(p+1) \f$ where \f$ S(p) \f$ is the first combination represented on the \f$ p \f$ 
            /// line (see \ref combination_for_line).
            /// The \f$ p \f$ line is the floor integer associated to the lower root of the following polynom:
            ///     \f[ p^2 - b.p + 2 c \f]
            /// with \f$ b = 2 sz + 1 \f$ and \f$ sz \f$ the size of the vector of ranks.
            /// \param c combination number.
            /// \return line number associated to the combination c.
            std::size_t line_for_combination(const std::size_t& c) const;

        public:
            /// \brief Constructor.
            /// \param ranks Vector of ranks.
            TriangularCombination(const std::vector<std::size_t>& ranks) : Combination(ranks) {};
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            TriangularCombination(const TriangularCombination& combination) : Combination(combination) {};
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            TriangularCombination& operator=(const TriangularCombination& combination);

            /// \brief Return the number of combinations.
            /// \return Number of combinations.
            virtual std::size_t size() const { return ranks.size() * (ranks.size() + 1) / 2; };

            /// \brief Return the combination associated to the given indices.
            /// \param row Row index.
            /// \param column Column index.
            /// \return Combination associated to the given indices.
            virtual std::size_t operator()(const std::size_t& row, const std::size_t& column) const;

            /// \brief Return the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual std::vector<std::size_t> get_ranks(const std::size_t& combination) const;           
    };

    /*!
     *  \brief Class used to manage all possible ranks' combinations given by a vector of ranks.
     * 
     *  \details All combination are given by a superior triangular matrix representation with a size equal to vector
     *  of ranks. The diagonnals terms are excluded. Its representation is the following matrix:
     *      \f{eqnarray*}{  
     *          \left [ \begin{array}{cccc}
     *               na   & (0,1) &  ...  & (0,n-1) \\
     *               na   &  na   &  ...  & (1,n-1) \\
     *               na   &  na   &  na   & (2,n-1) \\
     *               na   &  na   &  na   &  na 
     *          \end{array} \right ] 
     *      \f}
     */
    class SuperiorTriangularCombination : public Combination {
        private:
            /// \brief Determine the first combination represented on the p line.
            ///
            /// \details Determine the number of combination for \f$ p \f$ lines based on the following formula:
            /// \f[ S = \sum_{i=1}^{p} sz - i = \sum_{j=sz-p}^{sz} j = \frac{1}{2} p (2sz-1-p) \f]
            /// with \f$ sz \f$ the size of the vector of ranks.
            /// \param p Number of lines.
            /// \return First combination represented on the p line.
            std::size_t combination_for_line(const std::size_t& p) const;
            /// \brief Determine the line associated to a combination.
            ///
            /// \details Return the \f$ p \f$ line associated to the combination \f$ c \f$ such as:
            /// \f$ S(p) \leq c \leq S(p+1) \f$ where \f$ S(p) \f$ is the first combination represented on the \f$ p \f$ 
            /// line (see \ref combination_for_line).
            /// The \f$ p \f$ line is the floor integer associated to the lower root of the following polynom:
            ///     \f[ p^2 - b.p + 2 c \f]
            /// with \f$ b = 2 sz - 1 \f$ and \f$ sz \f$ the size of the vector of ranks.
            /// \param c combination number.
            /// \return line number associated to the combination c.
            std::size_t line_for_combination(const std::size_t& c) const;

        public:
            /// \brief Constructor.
            /// \param ranks Vector of ranks.
            SuperiorTriangularCombination(const std::vector<std::size_t>& ranks) : Combination(ranks) {};
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            SuperiorTriangularCombination(const SuperiorTriangularCombination& combination) : Combination(combination) {};
            /// \brief Copy constructor.
            /// \param combination Combination to copy.
            SuperiorTriangularCombination& operator=(const SuperiorTriangularCombination& combination);

            /// \brief Return the number of combinations.
            /// \return Number of combinations.
            virtual std::size_t size() const { return ranks.size() * (ranks.size() - 1) / 2; };

            /// \brief Return the combination associated to the given indices.
            /// \param row Row index.
            /// \param column Column index.
            /// \return Combination associated to the given indices.
            virtual std::size_t operator()(const std::size_t& row, const std::size_t& column) const;

            /// \brief Return the row and column indices associated to a combination.
            /// \param combination Combination.
            /// \return indices associated to the given combination.
            virtual std::vector<std::size_t> get_ranks(const std::size_t& combination) const;            
    };

};