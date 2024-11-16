#include <cassert>

#include "StressContainer.h"

using namespace amath;

StressContainer::StressContainer(const StressContainer& other) {
    this->_intensity_ = other._intensity_;
    this->_ratio_ = other._ratio_;
    this->_mean_ = other._mean_;
    this->_loads_ = other._loads_;
    this->_torsors_ = other._torsors_;
}

StressContainer& StressContainer::operator=(const StressContainer& other) {
    if (this != &other) {
        this->_intensity_ = other._intensity_;
        this->_ratio_ = other._ratio_;
        this->_mean_ = other._mean_;
        this->_loads_ = other._loads_;
        this->_torsors_ = other._torsors_;
    }
    return *this;
}

bool StressContainer::operator==(const StressContainer& other) {
    return _ratio_ == other._ratio_;
}

bool StressContainer::operator!=(const StressContainer& other) {
    return !(*this == other);
}

bool StressContainer::operator<=(const StressContainer& other) {
    return _ratio_ <= other._ratio_;
}

bool StressContainer::operator>=(const StressContainer& other) {
    return _ratio_ >= other._ratio_;
}

bool StressContainer::operator<(const StressContainer& other) {
    return _ratio_ < other._ratio_;
}

bool StressContainer::operator>(const StressContainer& other) {
    return _ratio_ > other._ratio_;
}

void StressContainer::set_intensity(const double& intensity, const std::size_t load, const std::size_t torsor) {
    this->_intensity_ = intensity;
    this->_ratio_ = intensity;
    this->_loads_.first = load;
    this->_loads_.second = load;
    this->_torsors_.first = torsor;
    this->_torsors_.second = torsor;
}

void StressContainer::set_range(const std::vector<double>& range, const combi_ranks& loads, const combi_ranks& torsors) {
    std::size_t sz = range.size();
    assert(sz <= 3 && "Cannot set StressContainer due to vector size");
    switch (sz) {
        case 1 :
            this->_intensity_ = range[0];
            this->_ratio_ = range[0];
            this->_mean_ = 0.;
            break;
        case 2 :
            this->_intensity_ = range[0];
            this->_ratio_ = range[1];
            this->_mean_ = 0.;
            break;
        case 3 :
            this->_intensity_ = range[0];
            this->_ratio_ = range[1];
            this->_mean_ = range[2];
            break;
        default:
            break;
    }

    this->_loads_ = loads;
    this->_torsors_ = torsors;
}

void StressContainer::store_max(const StressContainer& other) {
    if (*this < other)  *this = other;
}

