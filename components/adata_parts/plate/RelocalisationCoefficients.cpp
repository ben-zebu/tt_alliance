#include "RelocalisationCoefficients.h"

using namespace adata::parts;
            
double RelocalisationCoefficients::get_relocalization_coef(const double& angle, const std::string& type) const {
    auto it = _coefficients_.find(type);
    if (it == _coefficients_.end()) {
        error(translate("UNKNOWN_RELOCALIZATION_COEFFICIENT", type));
    }
    return it->second.get_yvalue(angle, "linear");
}

void RelocalisationCoefficients::set_a_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["a_phi"] = amath::Table(angles, coefs);
}

void RelocalisationCoefficients::set_b_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["b_phi"] = amath::Table(angles, coefs);
}

void RelocalisationCoefficients::set_c_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["c_phi"] = amath::Table(angles, coefs);
}

double RelocalisationCoefficients::get_a_phi(const double& angle) const {
    return get_relocalization_coef(angle, "a_phi");
}

double RelocalisationCoefficients::get_b_phi(const double& angle) const {
    return get_relocalization_coef(angle, "b_phi");
}

double RelocalisationCoefficients::get_c_phi(const double& angle) const {
    return get_relocalization_coef(angle, "c_phi");
}

std::shared_ptr<RelocalisationCoefficients> RelocalisationCoefficients::clone() const {
    std::shared_ptr<RelocalisationCoefficients> clone = std::make_shared<RelocalisationCoefficients>();
    for (const auto& coef : _coefficients_) {
        clone->_coefficients_[coef.first] = amath::Table(coef.second);
    }
    return clone;
}

void RelocalisationCoefficients::set_periodic_conditions() {
    if (_type_.empty()) throw std::invalid_argument("Cannot set_periodic_conditions for empty type !");
    

    // TODO
}
