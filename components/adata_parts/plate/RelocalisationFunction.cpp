#include <algorithm>

#include "List.h"
#include "RelocalisationFunction.h"

using namespace adata::parts;
            
double RelocalisationFunction::get_relocalization_coef(const double& angle, const std::string& type) const {
    auto it = _coefficients_.find(type);
    if (it == _coefficients_.end()) {
        error(translate("UNKNOWN_RELOCALIZATION_COEFFICIENT", type));
    }
    return it->second.get_yvalue(angle, "linear");
}

void RelocalisationFunction::build_relocalisation_function(const double& ph, const PlateCoefficients& coef1,
                                                           const PlateCoefficients& coef2) {
    if (coef1.Ph == coef2.Ph) error(translate("ERROR_SAME_PH", std::to_string(coef1.Ph)));
    double ph_ratio = (ph - coef1.Ph) / (coef2.Ph - coef1.Ph);
    if (ph_ratio < 0. || ph_ratio > 1.) error(translate("ERROR_PH_RATIO", std::to_string(ph_ratio)));

    std::vector<double> merged_angles = amath::merge_and_sort_unique(coef1.angles, coef2.angles);

    set_a_phi(merged_angles, build_interpolated_values("a_phi", coef1, coef2, merged_angles, ph_ratio));
    set_b_phi(merged_angles, build_interpolated_values("b_phi", coef1, coef2, merged_angles, ph_ratio));
    set_c_phi(merged_angles, build_interpolated_values("c_phi", coef1, coef2, merged_angles, ph_ratio));
}

std::vector<double> RelocalisationFunction::build_interpolated_values(const std::string& coef_type, 
                                                                      const PlateCoefficients& coef1, 
                                                                      const PlateCoefficients& coef2, 
                                                                      std::vector<double>& merged_angles, 
                                                                      double ph_ratio) const {
    std::vector<double> interpolated_values;

    // build tables using for interpolation
    amath::Table T1, T2;
    if (coef_type == "a_phi") {
        T1 = amath::Table(coef1.angles, coef1.a_phi);
        T2 = amath::Table(coef2.angles, coef2.a_phi);
    } else if (coef_type == "b_phi") {
        T1 = amath::Table(coef1.angles, coef1.b_phi);
        T2 = amath::Table(coef2.angles, coef2.b_phi);
    } else if (coef_type == "c_phi") {
        T1 = amath::Table(coef1.angles, coef1.c_phi);
        T2 = amath::Table(coef2.angles, coef2.c_phi);
    } else {
        throw std::invalid_argument("Unknown relocalisation coefficient type '" + coef_type + "' !");
    }

    // interpolate values
    for (const auto &angle : merged_angles) {
        double coef_T1 = T1.get_yvalue(angle, "linear");
        double coef_T2 = T2.get_yvalue(angle, "linear");
        interpolated_values.push_back(coef_T1 + ph_ratio * (coef_T2 - coef_T1));
    }
    return interpolated_values;
}

void RelocalisationFunction::set_a_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["a_phi"] = amath::Table(angles, coefs);
}

void RelocalisationFunction::set_b_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["b_phi"] = amath::Table(angles, coefs);
}

void RelocalisationFunction::set_c_phi(std::vector<double> angles, std::vector<double> coefs) {
    _coefficients_["c_phi"] = amath::Table(angles, coefs);
}

double RelocalisationFunction::get_a_phi(const double& angle) const {
    return get_relocalization_coef(angle, "a_phi");
}

double RelocalisationFunction::get_b_phi(const double& angle) const {
    return get_relocalization_coef(angle, "b_phi");
}

double RelocalisationFunction::get_c_phi(const double& angle) const {
    return get_relocalization_coef(angle, "c_phi");
}

std::shared_ptr<RelocalisationFunction> RelocalisationFunction::clone() const {
    std::shared_ptr<RelocalisationFunction> clone = std::make_shared<RelocalisationFunction>();
    for (const auto& coef : _coefficients_) {
        clone->_coefficients_[coef.first] = amath::Table(coef.second);
    }
    return clone;
}

void RelocalisationFunction::set_periodic_conditions() {
    if (_type_.empty()) throw std::invalid_argument("Cannot set_periodic_conditions for empty type !");
    

    // TODO
}
