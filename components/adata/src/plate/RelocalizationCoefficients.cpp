#include "RelocalizationCoefficients.h"

using namespace adata;
            

double RelocalizationCoefficients::get_relocalization_coef(const double& angle, const std::string& type) const {
    auto it = _coefficients_.find(type);
    if (it == _coefficients_.end()) {
        error(translate("UNKNOWN_RELOCALIZATION_COEFFICIENT", type));
    }
    return it->second.get_yvalue(angle, "linear");
}


void RelocalizationCoefficients::set_a_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["a_phi"] = amath::Table(angles, coefs);
}

void RelocalizationCoefficients::set_b_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["b_phi"] = amath::Table(angles, coefs);
}

void RelocalizationCoefficients::set_c_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["c_phi"] = amath::Table(angles, coefs);
}

double RelocalizationCoefficients::get_a_phi(const double& angle) const {
    return get_relocalization_coef(angle, "a_phi");
}

double RelocalizationCoefficients::get_b_phi(const double& angle) const {
    return get_relocalization_coef(angle, "b_phi");
}

double RelocalizationCoefficients::get_c_phi(const double& angle) const {
    return get_relocalization_coef(angle, "c_phi");
}