#include <limits>

#include "Coefficient.h"

using namespace amath;

Coefficient::Coefficient(const Table& table) {
    _table_ = table;
}

Coefficient::Coefficient(const Coefficient& coefficient) {
    _table_ = coefficient._table_;
}

Coefficient& Coefficient::operator=(const Coefficient& coefficient) {
    if (this != &coefficient) {
        _table_ = coefficient._table_;
    }
    return *this;
}

ConstantCoefficient::ConstantCoefficient(double value) {
    std::vector<double> xvalues = {std::numeric_limits<double>::min(), std::numeric_limits<double>::max()};
    std::vector<double> yvalues = {value, value};
    _table_ = Table(xvalues, yvalues);
}
