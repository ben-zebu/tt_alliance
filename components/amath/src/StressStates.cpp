#include <iostream>
#include <numeric>

#include "GlobalTimer.h"
#include "StressStates.h"
#include "TorsorCombination.h"

using namespace amath;

StressStates::~StressStates() {
    // resert the object in order to nullify pointers
    torsors_manager.reset();
}

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

void StressStates::set_active_torsors(const std::vector<bool>& active_torsors) { 
    torsors_manager.set_ptr_coefficients(CoefficientsMax, CoefficientsMin);
    torsors_manager.set_active_torsors(active_torsors); 
}

StressContainer StressStates::stress_intensity(const std::vector<size_t>& states_id) {
    StressContainer Sc_max;
    Stress Stotal;
    combi_ranks loads = {0, 0};

    std::vector<double> coefs( nb_torsors() );
    std::vector<bool> active_torsors( nb_torsors() );
    for (std::size_t i = 0; i < nb_torsors(); ++i) active_torsors[i] = true;
    
    set_active_torsors(active_torsors);

    for (std::size_t i = 0; i < states_id.size(); ++i) {
        std::size_t rk = states_id[i];
        loads.first = rk;
        for (std::size_t t = 0; t < torsors_manager.nb_combinaisons(rk); ++t) {
            Stotal = PrimaryStresses[rk];

            if ( torsors_manager.is_activate() ) {
                torsors_manager.get_coef(rk, t, coefs);
                Stotal = std::inner_product(coefs.begin(), coefs.end(), Torsors.begin(), Stotal);            
            }
            _maximum_equivalent_stress_(Sc_max, Stotal, 1., loads);
        }
    }
    return Sc_max;
}

StressContainer StressStates::stress_range(const Combination& explorer) {
    return stress_range_ratio(explorer, ConstantCoefficient(1.));
}

StressContainer StressStates::stress_range_ratio(const Combination& explorer, const Coefficient& coefficient) {
    StressContainer Sc_max;
    combi_ranks ranks;
    Stress Scumul, Stotal;
    std::vector<double> coefs( nb_torsors() );

    std::vector<bool> active_torsors( nb_torsors() );
    for (std::size_t i = 0; i < nb_torsors(); ++i) active_torsors[i] = true;
    
    set_active_torsors(active_torsors);

    for (std::size_t i = 0; i < explorer.size(); ++i) {
        explorer.ranks_by_ptr(i, ranks);
        double cc = get_interpolated_coeffient(coefficient, ranks);
        Scumul = PrimaryStresses[ranks.first] - PrimaryStresses[ranks.second];

        for (std::size_t t = 0; t < torsors_manager.nb_combinaisons(ranks); ++t) {
            Stotal = Scumul;

            if ( torsors_manager.is_activate() ) {
                torsors_manager.get_diff_coef(ranks, t, coefs);
                Stotal = std::inner_product(coefs.begin(), coefs.end(), Torsors.begin(), Stotal);
            }
            _maximum_equivalent_stress_(Sc_max, Stotal, cc, ranks);
        }
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
    for (std::size_t i = 0; i < PrimaryStresses.size(); ++i) {
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
        if ( ratio_max > Sr_max.get_ratio() ) ratio_max = Sr.tresca() / coef;
    }
    else {
        throw std::runtime_error("Invalid equivalent stress method.");
    }

    if (ratio_max > Sr_max.get_ratio()) {
        Sr_max.set_range({ratio_max * coef, ratio_max}, loads, {0,0});
        Sr_max.set_temperatures(Temperatures[loads.first], Temperatures[loads.second]);
    }
}

double StressStates::get_interpolated_coeffient(const Coefficient& coefficient, const combi_ranks& ranks) const {
    // case for undefined temperatures
    if (Temperatures.empty()) return 1.;

    double c1 = coefficient.get_yvalue(Temperatures[ranks.first]);
    double c2 = coefficient.get_yvalue(Temperatures[ranks.second]);
    return std::max(c1, c2);
}