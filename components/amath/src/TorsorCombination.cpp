#include <stdexcept>

#include "TorsorCombination.h"

using namespace amath;

void TorsorCombination::reset() {
    ptr_coef_max = nullptr;
    ptr_coef_min = nullptr;
    active_torsors.clear();
    _is_activate_ = false;
}

TorsorCombination::TorsorCombination(const TorsorCombination& other) {
    this->active_torsors = other.active_torsors;
    this->ptr_coef_max = other.ptr_coef_max;
    this->ptr_coef_min = other.ptr_coef_min;
    _is_activate_ = false;
}

void TorsorCombination::set_ptr_coefficients(std::vector<std::vector<double>>& CoefMax, 
                                                std::vector<std::vector<double>>& CoefMin) {
    if (CoefMax.size() != CoefMin.size()) {
        throw std::runtime_error("Invalid size for coefficients vector");
    }
    ptr_coef_max = &CoefMax;
    ptr_coef_min = &CoefMin;
    _is_activate_ = false;
}

void TorsorCombination::set_active_torsors(const std::vector<bool>& torsors) {
    active_torsors = torsors;
    cached_size = active_torsors.size();
    _is_activate_ = true;
    _check_();
}

void TorsorCombination::get_coef(std::size_t state, std::size_t tcomb, std::vector<double>& coefficients) const {
    double c1_max, c1_min;
    combi_ranks states = {state, state};

    if (coefficients.size() != cached_size) {
        throw std::runtime_error("Invalid size for coefficients vector");
    }

    std::size_t tree_rk = tcomb;

    for (std::size_t i = 0; i < cached_size; ++i) {
        std::size_t rk = cached_size - 1 - i;

        // set to zero for inactive torsor
        if (!active_torsors[rk]) { 
            coefficients[rk] = 0.; 
            continue;
        } 

        c1_max = (*ptr_coef_max)[state][rk];
        c1_min = (*ptr_coef_min)[state][rk];

        if (constant_coefficients(states, rk)) {
            coefficients[rk] = c1_max;
        }
        else {
            coefficients[rk] = (tree_rk % 2 == 0) ? c1_max : c1_min;
            tree_rk /= 2;
        }
    }
}

void TorsorCombination::get_combi_coef(combi_ranks states, std::size_t tcomb, const std::pair<double,double>& cl, 
                                       std::vector<double>& coefficients) const {
    double c1_max, c1_min;
    double c2_max, c2_min;

    if (coefficients.size() != cached_size) {
        throw std::runtime_error("Invalid size for coefficients vector");
    }

    std::size_t tree_rk = tcomb;

    for (std::size_t i = 0; i < cached_size; ++i) {
        std::size_t rk = cached_size - 1 - i;

        // set to zero for inactive torsor
        if (!active_torsors[rk]) { 
            coefficients[rk] = 0.; 
            continue;
        } 

        c1_max = cl.first*(*ptr_coef_max)[states.first][rk];
        c1_min = cl.first*(*ptr_coef_min)[states.first][rk];

        c2_max = cl.second*(*ptr_coef_max)[states.second][rk];
        c2_min = cl.second*(*ptr_coef_min)[states.second][rk];

        if (constant_coefficients(states, i)) {
            coefficients[rk] = c1_max + c2_max;
        }
        else {
            coefficients[rk] = (tree_rk % 2 == 0) ? c1_max + c2_min : c1_min + c2_max;
            tree_rk /= 2;
        }
    }
}

void TorsorCombination::get_diff_coef(combi_ranks states, std::size_t tcomb, std::vector<double>& coefficients) const {
    get_combi_coef(states, tcomb, {1., -1.}, coefficients);
}

void TorsorCombination::get_sum_coef(combi_ranks states, std::size_t tcomb, std::vector<double>& coefficients) const {
    get_combi_coef(states, tcomb, {1., 1.}, coefficients);
}

std::size_t TorsorCombination::nb_combinaisons(std::size_t state) const {
    combi_ranks states = {state, state};
    return nb_combinaisons(states);
}

std::size_t TorsorCombination::nb_combinaisons(const combi_ranks& states) const {
    std::size_t nb_comb = 1;
    // case for undefined torsors status 
    if ( !is_activate() ) return nb_comb;

    for (std::size_t i = 0; i < cached_size; ++i) {
        // only active torsors are taken into account
        if (!active_torsors[i]) continue;

        if ( !constant_coefficients(states, i) ) nb_comb *= 2;
    }
    return nb_comb;
}

//
// Private functions
//
bool TorsorCombination::constant_coefficients(const combi_ranks& states, std::size_t torsor_rk) const {
    double cmax = abs( (*ptr_coef_max)[states.first][torsor_rk] ) + abs( (*ptr_coef_max)[states.second][torsor_rk] );
    double cmin = abs( (*ptr_coef_min)[states.first][torsor_rk] ) + abs( (*ptr_coef_min)[states.second][torsor_rk] );
    if (abs(cmax - cmin) < COEFFICIENT_TOLERANCE) {
        return true;
    }
    return false;
}

void TorsorCombination::_check_() const {
    if (cached_size == 0) {
        throw std::runtime_error("Failed check integrity");
    }
    if (ptr_coef_max == nullptr || ptr_coef_min == nullptr) {
        throw std::runtime_error("Undefined Coefficients vectors");
    }
    if (cached_size != (*ptr_coef_max)[0].size()) {
        throw std::runtime_error("Invalid size for coefficients vector");
    }
}
