#include <stdexcept>

#include "Stress.h"

using namespace amath;

//
// Constructors and copy constructor
//
            
Stress& Stress::operator=(const Stress& other) {
    if (this != &other) components = other.components;
    return *this;
}

Stress::Stress(const std::vector<double>& components) {
    assert(components.size() == STRESS_SIZE && "Stress constructor: wrong number of components");
    std::copy(components.begin(), components.end(), this->components.begin());   
}

void Stress::zero() {
    std::fill(components.begin(), components.end(), 0.0);
}

//
// Operators overloading
//

namespace amath {
    Stress operator+(const Stress& s1, const Stress& s2) { 
        Stress s(s1);
        return s += s2;
    }
    Stress operator-(const Stress& s1, const Stress& s2) { 
        Stress s(s1);
        return s -= s2;
    }
    Stress operator*(const double& scalar, const Stress& s1) { 
        Stress s(s1);
        return s *= scalar;
    }

    std::ostream& operator<<(std::ostream& flux, const Stress& stress) {
        stress.print(flux);
        return flux;
    }

}


Stress& Stress::operator+=(const Stress& other) {
    for  (std::size_t i = 0; i < this->size(); ++i) {
        components[i] += other[i];
    }
    return *this;
}

Stress& Stress::operator-=(const Stress& other) {
    for  (std::size_t i = 0; i < this->size(); ++i) {
        components[i] -= other[i];
    }
    return *this;
}

Stress& Stress::operator*=(const double& scalar) {
    for (std::size_t i = 0; i < this->size(); ++i) {
        components[i] *= scalar;
    }    
    return *this;
}

bool Stress::operator==(const Stress& other) {
    for (size_t i = 0; i < this->size(); ++i) {
        if (components[i] != other.components[i]) {
            return false;
        }
    }
    return true;
}

bool Stress::operator!=(const Stress& other) {
    return !(*this == other);
}

bool Stress::is_diag() const {
    for (size_t i = this->size()/2; i < this->size(); ++i) {
        if (std::abs(components[i]) >= STRESS_TOLERANCE) {
            return false;
        }
    }
    return true;
}

void Stress::print(std::ostream& flux) const {
    std::string ss = "[ ";
    for (std::size_t i=0; i < size(); ++i) {
        ss += std::to_string(components[i]);
        if (i != size() - 1) ss += ", ";
    }
    flux << ss << " ]";
}

//
// Stress invariants
//
std::array<double, STRESS_SIZE/2> Stress::principal_stresses() const {
    double s1, s2, s3;
    getPrincipalStresses(s1, s2, s3);
    return {s1, s2, s3};
}

double Stress::tresca() const {
    double s1, s2, s3;
    getPrincipalStresses(s1, s2, s3);
    return std::abs(s1 - s3);
}

double Stress::mises() const {
    const auto& s = this->components;    
    double m = 0;
    m += std::pow(s[0] - s[1], 2);
    m += std::pow(s[1] - s[2], 2);
    m += std::pow(s[2] - s[0], 2);
    m += 6.0 * (s[3]*s[3] + s[4]*s[4] + s[5]*s[5]);
    return std::sqrt(0.5 * m);
}

double Stress::reduced_mises() const {
    return VM_FACTOR * mises();
}

//
// Cardan method 
//
void Stress::getPrincipalStresses(double& s1, double& s2, double& s3) const {
    const auto& s = this->components;
    
    // Compute invariants
    double I1 = s[0] + s[1] + s[2];
    double I2 = s[0]*s[1] + s[1]*s[2] + s[2]*s[0] - (s[3]*s[3] + s[4]*s[4] + s[5]*s[5]);
    double I3 = s[0]*s[1]*s[2] + 2*s[3]*s[4]*s[5] - (s[0]*s[5]*s[5] + s[1]*s[4]*s[4] + s[2]*s[3]*s[3]);
    
    // Solve cubic equation using Cardan method
    double p = I2 - I1*I1/3.0;
    double q = 2.0*I1*I1*I1/27.0 - I1*I2/3.0 + I3;
    double r = -I1/3.0;

    solveCardan(p, q, r, s1, s2, s3);    
    sortEigenvalues(s1, s2, s3);
}

void Stress::sortEigenvalues(double& a, double& b, double& c) {
    if (a < b) std::swap(a, b);
    if (b < c) std::swap(b, c);
    if (a < b) std::swap(a, b);
}

void Stress::solveCardan(const double p, const double q, const double r,
                        double& x1, double& x2, double& x3) {
    const double pi = M_PI;
    double D = q*q/4.0 + p*p*p/27.0;
    
    if (D > 0) {
        double u = std::cbrt(-q/2.0 + std::sqrt(D));
        double v = std::cbrt(-q/2.0 - std::sqrt(D));
        x1 = u + v + r;
        x2 = -0.5*(u + v) + r;
        x3 = x2;
    } else {
        double phi = std::acos(-q/(2.0*std::sqrt(-p*p*p/27.0)));
        double rho = 2.0*std::sqrt(-p/3.0);
        x1 = rho*std::cos(phi/3.0) + r;
        x2 = rho*std::cos((phi + 2.0*pi)/3.0) + r;
        x3 = rho*std::cos((phi + 4.0*pi)/3.0) + r;
    }
}
