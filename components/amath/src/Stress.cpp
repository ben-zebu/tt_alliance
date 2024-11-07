#include "Stress.h"

using namespace amath;

//
// Constructors and copy constructor
//

Stress::Stress(const Vector6d& components) {
    this->components = components;
};

Stress::Stress(const Stress& other) {
    this->components = other.components;
};
            
Stress& Stress::operator=(const Stress& other) {
    this->components = other.components;
    return *this;
}

Stress::Stress(const std::initializer_list<double>& components) {
    if (components.size() != size()) {
        throw std::invalid_argument("Stress constructor: wrong number of components");
    }
    int i = 0;
    for (auto value : components) {
        this->components[i++] = value;
    }    
}

//
// Operators overloading
//
Stress operator+(const Stress& s1, const Stress& s2) {
    return Stress(s1.get_components() + s2.get_components());
}      

Stress operator-(const Stress& s1, const Stress& s2) {
    return Stress(s1.get_components() - s2.get_components());
}

//Stress operator*(const double& scalar, const Stress& s) {
//    return Stress(scalar * s.get_components());
//}

Stress& Stress::operator+=(const Stress& other) {
    this->components += other.components;
    return *this;
}
Stress& Stress::operator-=(const Stress& other) {
    this->components -= other.components;
    return *this;
}

Stress& Stress::operator*=(const double& scalar) {
    this->components *= scalar;
    return *this;
}

bool Stress::operator==(const Stress& other) {
    for (size_t i = 0; i < this->size(); i++) {
        if (this->components[i] != other.components[i]) {
            return false;
        }
    }
    return true;
}

bool Stress::operator!=(const Stress& other) {
    return !(*this == other);
}

bool Stress::is_diag() const {
    for (size_t i = this->size()/2; i < this->size(); i++) {
        if (std::abs(components[i]) >= STRESS_TOLERANCE) {
            return false;
        }
    }
    return true;
}
