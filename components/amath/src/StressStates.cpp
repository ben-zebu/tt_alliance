#include <iostream>

#include "GlobalTimer.h"
#include "StressStates.h"

using namespace amath;

StressContainer global_tmp_Sr;

void StressStates::reset() {
    PrimaryStresses.clear();
    SecondaryStresses.clear();
    Temperatures.clear();
    Torsors.clear();
    CoefficientsMax.clear();
    CoefficientsMin.clear();
}

void StressStates::set_equivalent_stress_method(const std::string& method) { 
    if (method != "tresca" && method != "mises" && method != "reduced_mises") {
        throw std::invalid_argument("Invalid equivalent stress method");
    }
    equivalent_stress_method = method; 
}

void StressStates::add_stress(const Stress& primary_stress, const Stress& secondary_stress) {
    PrimaryStresses.push_back(primary_stress);
    SecondaryStresses.push_back(secondary_stress);
}

void StressStates::add_temperature(const double& temperature) {
    Temperatures.push_back(temperature);
}

void StressStates::add_torsor(const Stress& torsor) {
    Torsors.push_back(torsor);
}

void StressStates::add_torsor_coefficients(const std::vector<double>& cmax, const std::vector<double>& cmin) {
    CoefficientsMax.push_back(cmax);
    CoefficientsMin.push_back(cmin);
}

StressContainer StressStates::stress_intensity(const std::vector<size_t>& states_id) {
    StressContainer Sc_max;
    combi_ranks loads = {0, 0};
    for (size_t i = 0; i < states_id.size(); i++) {
        size_t rk = states_id[i];
        loads.first = rk;
        _maximum_equivalent_stress_(Sc_max, PrimaryStresses[rk], 1., loads);
    }
    return Sc_max;
}

StressContainer StressStates::stress_range(const std::vector<size_t>& states_id) {
    return stress_range_ratio(states_id, ConstantCoefficient(1.));
}

StressContainer StressStates::stress_range_ratio(const std::vector<size_t>& states_id, const Coefficient& coefficient) {
    StressContainer Sc_max;
    for (size_t i = 0; i < states_id.size()-1; i++) {
        size_t rk1 = states_id[i];
        double c1 = Temperatures.empty() ? 1. : coefficient.get_yvalue(Temperatures[rk1]);

        for (size_t j = i+1; j < states_id.size(); j++) {
            size_t rk2 = states_id[j];
            double c2 = Temperatures.empty() ? 1. : coefficient.get_yvalue(Temperatures[rk2]);            
            
            Stress Sr = PrimaryStresses[rk1] - PrimaryStresses[rk2];
            _maximum_equivalent_stress_(Sc_max, Sr, std::max(c1, c2), {rk1, rk2});
        }
    }

    // Determine the mean stress associated with the maximum stress range
    /*
        TODO:
    */
    return Sc_max;
}

StressContainer StressStates::stress_range(const Combination& explorer) {
    return stress_range_ratio(explorer, ConstantCoefficient(1.));
}

StressContainer StressStates::stress_range_ratio(const Combination& explorer, const Coefficient& coefficient) {
    StressContainer Sc_max;
    combi_ranks ranks;
    double c1, c2;
    double coef = 1.;
    Stress Sr;
    for (size_t i = 0; i < explorer.size(); i++) {
        explorer.ranks_by_ptr(i, ranks);
        if (!Temperatures.empty()) {
            c1 = coefficient.get_yvalue(Temperatures[ranks.first]);
            c2 = coefficient.get_yvalue(Temperatures[ranks.second]);
            coef = std::max(c1, c2);
        }
        Sr = PrimaryStresses[ranks.first] - PrimaryStresses[ranks.second];
        _maximum_equivalent_stress_(Sc_max, Sr, coef, ranks);
    }

    // Determine the mean stress associated with the maximum stress range
    /*
        TODO:
    */
    return Sc_max;
}

//
// PRIVATE METHODS
//
void StressStates::_check_() {
    if (!SecondaryStresses.empty() && PrimaryStresses.size() != SecondaryStresses.size()) {
        throw std::runtime_error("PrimaryStresses and SecondaryStresses must have the same size.");
    }
    if (!Temperatures.empty() && PrimaryStresses.size() != Temperatures.size()) {
        throw std::runtime_error("PrimaryStresses and Temperatures must have the same size.");
    }
    if (!Torsors.empty() && (CoefficientsMax.empty() || CoefficientsMin.empty())) {
        throw std::runtime_error("CoefficientsMax and CoefficientsMin must be defined for each Torsors.");
    }
    if (!CoefficientsMax.empty() && PrimaryStresses.size() != CoefficientsMax.size()) {
        throw std::runtime_error("PrimaryStresses and CoefficientsMax must have the same size.");
    }
    if (!CoefficientsMin.empty() && PrimaryStresses.size() != CoefficientsMin.size()) {
        throw std::runtime_error("PrimaryStresses and CoefficientsMin must have the same size.");
    }
    for (size_t i = 0; i < PrimaryStresses.size(); ++i) {
        if (CoefficientsMax[i].size() != Torsors.size()) {
            throw std::runtime_error("For each state, CoefficientsMax must have the same size as Torsors.");
        }
        if (CoefficientsMin[i].size() != Torsors.size()) {
            throw std::runtime_error("For each state, CoefficientsMin must have the same size as Torsors.");
        }
    }
}

void StressStates::_maximum_equivalent_stress_(StressContainer& Sr_max, const Stress& Sr, const double coef, const combi_ranks& loads) {
    double ratio_max = 0.;
    if (equivalent_stress_method == "tresca") {
        ratio_max = Sr.tresca() / coef;
    }
    else if (equivalent_stress_method == "mises") {
        ratio_max = Sr.mises() / coef;
    }
    else if (equivalent_stress_method == "reduced_mises") {
        ratio_max = Sr.reduced_mises() / coef;
    }
    else {
        throw std::runtime_error("Invalid equivalent stress method.");
    }

    if (ratio_max > Sr_max.get_ratio() && equivalent_stress_method == "reduced_mises") {
        ratio_max = Sr.tresca() / coef;
    }

    global_tmp_Sr.set_range({ratio_max * coef, ratio_max}, loads, {0,0});
    Sr_max.store_max(global_tmp_Sr);
}
