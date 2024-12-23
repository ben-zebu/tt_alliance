#include <cassert>

#include "StressContainer.h"

using namespace amath;

StressContainer::StressContainer(const StressContainer& other) {
    this->_intensity_ = other._intensity_;
    this->_ratio_ = other._ratio_;
    this->_mean_ = other._mean_;
    this->_loads_ = other._loads_;
    this->_torsor_ = other._torsor_;
    this->_temperatures_ = other._temperatures_;
}

StressContainer& StressContainer::operator=(const StressContainer& other) {
    if (this != &other) {
        this->_intensity_ = other._intensity_;
        this->_ratio_ = other._ratio_;
        this->_mean_ = other._mean_;
        this->_loads_ = other._loads_;
        this->_torsor_ = other._torsor_;
        this->_temperatures_ = other._temperatures_;
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
    this->_torsor_ = torsor;
}

void StressContainer::set_range(const std::vector<double>& range, const combi_ranks& loads, const std::size_t torsor) {
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
    this->_torsor_ = torsor;
}

void StressContainer::set_temperatures(const double& T1, const double& T2) {
    _temperatures_.first = T1;
    _temperatures_.second = T2;
}

void StressContainer::store_max(const StressContainer& other) {
    if (*this < other)  *this = other;
}

StressIntensity StressContainer::get_intensity() const {
    StressIntensity result;
    result.set(_intensity_, _loads_.first, _torsor_);
    return result;
}

StressRange StressContainer::get_range() const {
    StressRange result;
    result.set(_intensity_, _mean_, _ratio_, _loads_, _torsor_);
    return result;
}
