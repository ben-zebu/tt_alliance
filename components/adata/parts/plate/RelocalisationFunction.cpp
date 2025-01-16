#include <algorithm>

#include "List.h"
#include "RelocalisationFunction.h"

using namespace adata::parts;

RelocalisationFunction::RelocalisationFunction(const PlateCoefficients& plate_coef) {
    _coefficients_["a_phi"] = amath::Table(plate_coef.angles, plate_coef.a_phi);
    _coefficients_["b_phi"] = amath::Table(plate_coef.angles, plate_coef.b_phi);
    _coefficients_["c_phi"] = amath::Table(plate_coef.angles, plate_coef.c_phi);
}

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

    RelocalisationFunction reloc_f1(coef1);
    RelocalisationFunction reloc_f2(coef2);
    reloc_f1.set_periodic_conditions();
    reloc_f2.set_periodic_conditions();

    set_a_phi(merged_angles, build_interpolated_values("a_phi", reloc_f1, reloc_f2, merged_angles, ph_ratio));
    set_b_phi(merged_angles, build_interpolated_values("b_phi", reloc_f1, reloc_f2, merged_angles, ph_ratio));
    set_c_phi(merged_angles, build_interpolated_values("c_phi", reloc_f1, reloc_f2, merged_angles, ph_ratio));
}

std::vector<double> RelocalisationFunction::build_interpolated_values(const std::string& coef_type, 
                                                                      const RelocalisationFunction& reloc_f1, 
                                                                      const RelocalisationFunction& reloc_f2, 
                                                                      std::vector<double>& merged_angles, 
                                                                      double ph_ratio) const {
    std::vector<double> interpolated_values;

    // interpolate values
    for (const auto &angle : merged_angles) {
        double coef_T1 = reloc_f1.get_coef(coef_type, angle);
        double coef_T2 = reloc_f2.get_coef(coef_type, angle);
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

double RelocalisationFunction::get_coef(const std::string& type, const double& angle) const {
    return get_relocalization_coef(angle, type);
}


std::shared_ptr<RelocalisationFunction> RelocalisationFunction::clone() const {
    std::shared_ptr<RelocalisationFunction> clone = std::make_shared<RelocalisationFunction>();
    for (const auto& coef : _coefficients_) {
        clone->_coefficients_[coef.first] = amath::Table(coef.second);
    }
    return clone;
}

void RelocalisationFunction::set_periodic_conditions_up() {  
    if (_coefficients_.begin()->second.get_xmax() > 90.) return;

    // a_phi and b_phi are symmetric between [0, 90] and [90, 180]
    // c_phi is anti-symmetric between between [0, 90] and [90, 180]
    std::vector<double> angles = get_angles();
    std::reverse(angles.begin(), angles.end());

    std::vector<double> sym_angles;
    std::vector<double> sym_a_phi, sym_b_phi, sym_c_phi;
    for (const auto& angle : angles) {
        sym_angles.push_back(180. - angle);
        sym_a_phi.push_back(_coefficients_["a_phi"].get_yvalue(angle, "linear"));
        sym_b_phi.push_back(_coefficients_["b_phi"].get_yvalue(angle, "linear"));
        sym_c_phi.push_back(-1.*_coefficients_["c_phi"].get_yvalue(angle, "linear"));
    }
    _coefficients_["a_phi"].expand(sym_angles, sym_a_phi);
    _coefficients_["b_phi"].expand(sym_angles, sym_b_phi);
    _coefficients_["c_phi"].expand(sym_angles, sym_c_phi);

}

void RelocalisationFunction::set_periodic_conditions_down() {  
    if (_coefficients_.begin()->second.get_xmax() > 180.) return;

    // a_phi, b_phi and c_phi are symmetric between [0, 180] and [180, 360]
    std::vector<double> angles = get_angles();
    std::reverse(angles.begin(), angles.end());

    std::vector<double> sym_angles;
    std::vector<double> sym_a_phi, sym_b_phi, sym_c_phi;
    for (const auto& angle : angles) {
        sym_angles.push_back(360. - angle);
        sym_a_phi.push_back(_coefficients_["a_phi"].get_yvalue(angle, "linear"));
        sym_b_phi.push_back(_coefficients_["b_phi"].get_yvalue(angle, "linear"));
        sym_c_phi.push_back(_coefficients_["c_phi"].get_yvalue(angle, "linear"));
    }
    _coefficients_["a_phi"].expand(sym_angles, sym_a_phi);
    _coefficients_["b_phi"].expand(sym_angles, sym_b_phi);
    _coefficients_["c_phi"].expand(sym_angles, sym_c_phi);
}


void RelocalisationFunction::set_periodic_conditions() {
    if (_type_.empty()) throw std::invalid_argument("Cannot set_periodic_conditions for empty type !");
    
    // Complete coefficients for angles between [90, 180] based on [0, 90]
    set_periodic_conditions_up();

    // Complete coefficients for angles between [180, 360] based on [0, 180]
    set_periodic_conditions_down();
}
