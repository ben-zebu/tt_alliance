#pragma once

#include <string>
#include <vector>

namespace amath {


    /// \brief Represents a 2D table of double values.
    ///
    /// The Table class provides a 2D table of double values. It is implemented using a 2D vector of vectors.
    /// The table can be initialized with a size and filled with zeros or with a given value.
    /// It also supports various operations such as adding, subtracting, multiplying, and dividing tables.
    class Table {
        private:
            /// \brief The vector of abciss values
            std::vector<double> xvalues;
            /// \brief The vector of ordinate values
            std::vector<double> yvalues;

            /// \brief Return the maximum value in a vector of values.
            double get_max_value(const std::vector<double>& values) const;
            /// \brief Return the minimum value in a vector of values.
            double get_min_value(const std::vector<double>& values) const;


            /// \brief Return the linear interpolated ordinate at a given abciss value.
            /// \param[in] x The abciss value.
            /// \return The interpolated ordinate value.
            double get_linear_yvalue(double x) const;
            /// \brief Return the linear interpolated abciss at a given ordinate value.
            /// \param[in] y The ordinate value.
            /// \return The interpolated abciss value.
            double get_linear_xvalue(double y) const;
            /// \brief Return the logarithmic interpolated ordinate at a given abciss value.
            /// \param[in] x The abciss value.
            /// \return The interpolated ordinate value.
            double get_logarithmic_yvalue(double x) const;
            /// \brief Return the logarithmic interpolated abciss at a given ordinate value.
            /// \param[in] y The ordinate value.
            /// \return The interpolated abciss value.
            double get_logarithmic_xvalue(double y) const;

            /// \brief Check if the table is valid.
            void _check_() const;

        public:
            /// \brief Default constructor for the Table class.
            Table() = default;
            /// \brief Constructor for the Table class with a given size. All values are initialized to zero.
            /// \param[in] size The size of the table.
            Table(size_t size);
            /// \brief Constructor for the Table class based on abciss values and ordinates values.
            /// \param[in] abciss The abciss values.
            /// \param[in] ordinates The ordinates values.
            Table(const std::vector<double>& abciss, const std::vector<double>& ordinates);
            /// \brief Copy Constructor for the Table class.
            /// \param[in] table The object copy from Table class.
            Table(const Table& table);
            /// \brief Copy constructor for the Table class with = operator.
            Table& operator=(const Table& table);
            
            /// \brief Expand the table with other values.
            /// \param[in] abciss additional abciss values.
            /// \param[in] ordinates additional ordinates values.
            void expand(const std::vector<double>& abciss, const std::vector<double>& ordinates);

            /// \brief Return the size of the table.
            /// \return The size of the table.
            size_t size() const { return xvalues.size(); }
            /// \brief Return the maximum abciss value.
            /// \return The maximum abciss value.
            double get_xmax() const  { return get_max_value(xvalues); }
            /// \brief Return the minimum abciss value.
            /// \return The minimum abciss value.
            double get_xmin() const  { return get_min_value(xvalues); }
            /// \brief Return the maximum ordinates value.
            /// \return The maximum ordinates value.
            double get_ymax() const  { return get_max_value(yvalues); }
            /// \brief Return the minimum ordinates value.
            /// \return The minimum ordinates value.
            double get_ymin() const  { return get_min_value(yvalues); }
            /// \brief Return the range abciss value.
            /// \return The range abciss value.
            std::vector<double> get_xrange() const;
            /// \brief Return the range ordinates value.
            /// \return The range ordinates value.
            std::vector<double> get_yrange() const;
            /// \brief Return the abciss values.
            /// \return The abciss values.
            std::vector<double> get_xvalues() const { return xvalues; }
            /// \brief Return the ordinates values.
            /// \return The ordinates values.
            std::vector<double> get_yvalues() const { return yvalues; }

            /// \brief Return the interpolated ordinate at a given abciss value.
            /// \param[in] x The abciss value.
            /// \param[in] method The interpolation method.
            /// \return The interpolated ordinate value.
            double get_yvalue(double x, const std::string& method) const;
            /// \brief Return the interpolated abciss at a given ordinate value.
            /// \param[in] y The ordinate value.
            /// \param[in] method The interpolation method.
            /// \return The interpolated abciss value.
            double get_xvalue(double y, const std::string& method) const;
    };
}