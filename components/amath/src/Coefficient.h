#pragma once

#include "Table.h"

namespace amath {
    /// \brief Represents a coefficient that can depend on a parameter.
    ///
    /// This class represents a physical coefficient that can depend on a parameter. It provvides functions used to 
    /// interpolate table.
    class Coefficient {

        protected:
            /// \brief Internal table used to store data associated to the coefficient.
            Table _table_;

        public:

            /// \brief Default constructor for the Coefficient class.
            Coefficient() = default;
            /// \brief Constructor for the Coefficient class with a given table.
            Coefficient(const Table& table);
            /// \brief Copy constructor for the Coefficient class.
            Coefficient(const Coefficient& coefficient);
            /// \brief Copy assignment operator for the Coefficient class.
            Coefficient& operator=(const Coefficient& coefficient);
            
            /// \brief Pure virtual function used to return the y-value of the coefficient at a given x-value.
            virtual double get_yvalue(double x) const = 0;
            /// \brief Pure virtual function used to return the x-value of the coefficient at a given y-value.
            virtual double get_xvalue(double y) const = 0;
    };

    /// \brief Represents a coefficient depending on a parameter. A linear interpolation is used to compute the 
    /// coefficient for a given parameter.
    class LinearCoefficient : public Coefficient {
        public:

            /// \brief Default constructor for the LinearCoefficient class.
            LinearCoefficient() = default;
            /// \brief Constructor for the LinearCoefficient class with a given table.
            LinearCoefficient(const Table& table) : Coefficient(table) {};
            /// \brief Copy constructor for the LinearCoefficient class.
            LinearCoefficient(const Coefficient& coefficient) : Coefficient(coefficient) {};
            /// \brief Copy assignment operator for the LinearCoefficient class.
            LinearCoefficient& operator=(const LinearCoefficient& other);

            double get_yvalue(double x) const override {
                return _table_.get_yvalue(x, "linear");
            }
            double get_xvalue(double y) const override {
                return _table_.get_xvalue(y, "linear");
            }
    };

    /// \brief Represents a coefficient depending on a parameter. A logarithmic interpolation is used to compute the 
    /// coefficient for a given parameter.
    class LogarithmicCoefficient : public Coefficient {
        public:

            /// \brief Default constructor for the LogarithmicCoefficient class.
            LogarithmicCoefficient() = default;
            /// \brief Constructor for the LogarithmicCoefficient class with a given table.
            LogarithmicCoefficient(const Table& table) : Coefficient(table) {};
            /// \brief Copy constructor for the LogarithmicCoefficient class.
            LogarithmicCoefficient(const Coefficient& coefficient) : Coefficient(coefficient) {};
            /// \brief Copy assignment operator for the LogarithmicCoefficient class.
            LogarithmicCoefficient& operator=(const LogarithmicCoefficient& other);

            double get_yvalue(double x) const override {
                return _table_.get_yvalue(x, "logarithmic");
            }
            double get_xvalue(double y) const override {
                return _table_.get_xvalue(y, "logarithmic");
            }
    };

    /// \brief Represents a constant coefficient.
    class ConstantCoefficient : public LinearCoefficient {
        public:
            /// \brief Default constructor for the ConstantCoefficient class.
            ConstantCoefficient() = default;
            /// \brief Constructor for the ConstantCoefficient class with a given table.
            ConstantCoefficient(const Table& table) : LinearCoefficient(table) {};
            /// \brief Copy constructor for the Coefficient class.
            ConstantCoefficient(const Coefficient& coefficient) : LinearCoefficient(coefficient) {};
            /// \brief Copy assignment operator for the Coefficient class.
            ConstantCoefficient& operator=(const ConstantCoefficient& other);        
            /// \brief Constructor for the ConstantCoefficient class based on a given value.
            ConstantCoefficient(double value);

            /// \brief Return the value of the coefficient.
            double get() const { return _table_.get_ymax(); }
    };

}