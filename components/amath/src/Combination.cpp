#include <algorithm>
#include <cassert>
#include <cmath>

#include <iostream>
#include <stdexcept>

#include "Combination.h"

using namespace amath;

//
// Combination methods
//
Combination::Combination(const std::vector<std::size_t>& ranks) {
    this->ranks = ranks;
    sort_ranks();
}

Combination::Combination(const std::vector<std::size_t>& rks_1, const std::vector<std::size_t>& rks_2) {
    this->ranks = rks_1;
    this->ranks.insert(this->ranks.end(), rks_2.begin(), rks_2.end());
}

Combination::Combination(const Combination& combination) {
    this->ranks = combination.ranks;
}

Combination& Combination::operator=(const Combination& combination) {
    if (this != &combination) {
        this->ranks = combination.ranks;
    }
    set_cached_size();
    return *this;
}

void Combination::sort_ranks() {
    std::sort(this->ranks.begin(), this->ranks.end());
    this->ranks.erase(std::unique(this->ranks.begin(), this->ranks.end()), this->ranks.end());
}

//
// RectangularCombination methods
//

RectangularCombination::RectangularCombination(const std::vector<std::size_t>& ranks) : Combination(ranks) {
    nb_rows = ranks.size();
    nb_columns = ranks.size();
    set_cached_size();
}

RectangularCombination::RectangularCombination(const std::vector<std::size_t>& rks_1, 
                                               const std::vector<std::size_t>& rks_2) : Combination(rks_1, rks_2) {
    nb_rows = rks_1.size();
    nb_columns = rks_2.size();
    set_cached_size();
}

RectangularCombination& RectangularCombination::operator=(const RectangularCombination& combination) {
    if (this != &combination) {
        this->ranks = combination.ranks;
        this->nb_rows = combination.nb_rows;
        this->nb_columns = combination.nb_columns;
    }
    set_cached_size();
    return *this;
}

std::size_t RectangularCombination::operator()(const std::size_t& row, const std::size_t& column) const {
    return row * nb_columns + column;
}

combi_ranks RectangularCombination::get_ranks(const std::size_t& combination) const {
    return {combination / nb_columns, combination % nb_columns};
}

void RectangularCombination::ranks_by_ptr(const std::size_t& combination, combi_ranks& ranks) const {
    ranks.first = combination / nb_columns;
    ranks.second = combination % nb_columns;
}

//
// TriangularCombination methods
//
TriangularCombination::TriangularCombination(const std::vector<std::size_t>& ranks, bool on_diag)
                                                         : Combination(ranks) { 
    set_type(on_diag); 
}
TriangularCombination::TriangularCombination(const TriangularCombination& combination, bool on_diag)
                                                         : Combination(combination) { 
    set_type(on_diag);
}

void TriangularCombination::set_type(bool on_diag) { 
    _on_diag_ = on_diag; 
    set_cached_size();
}

void TriangularCombination::set_cached_size() {
    std::size_t n = ranks.size();
    
    // check maximum number represented by std::size_t and compare it to the number of combinations
    double max_comb = get_type() ? n * (n + 1) / 2 : n * (n - 1) / 2;
    std::size_t size_max = std::numeric_limits<std::size_t>::max();
    if (max_comb > size_max) {
        throw std::runtime_error("Outbound for maximum rank " + std::to_string(n));
    }
    
    cached_size = get_type() ? n * (n + 1) / 2 : n * (n - 1) / 2;
} 

TriangularCombination& TriangularCombination::operator=(const TriangularCombination& combination) {
    if (this != &combination) {
        this->ranks = combination.ranks;
    }
    set_cached_size();
    set_type( combination.get_type() );
    return *this;
}

constexpr std::size_t TriangularCombination::combination_for_line(const std::size_t& p, const std::size_t n) const { 
    return (p + 1) * (2 * diag_shift(n) - p) / 2;
}

constexpr std::size_t TriangularCombination::line_for_combination(const std::size_t& combination, const std::size_t n) const {
    double dd = static_cast<double>(diag_shift(n));
    double b = 1 + 2 * dd;
    double c = 2 * (combination + 1);
    double delta = b * b - 4 * c;
    return std::size_t( std::ceil((b - std::sqrt(delta))*0.5) - 1);
}

std::size_t TriangularCombination::operator()(const std::size_t& row, const std::size_t& column) const {
    std::size_t n = ranks.size();
    std::size_t p = std::min(row, column);
    std::size_t q = std::max(row, column);
    return combination_for_line(p, n) + q - n;
}

combi_ranks TriangularCombination::get_ranks(const std::size_t& combination) const {
    combi_ranks ranks;
    ranks_by_ptr(combination, ranks);
    return {ranks.first, ranks.second};
}

void TriangularCombination::ranks_by_ptr(const std::size_t& combination, combi_ranks& ranks) const {
    std::size_t n = this->ranks.size();
    std::size_t p = line_for_combination(combination, n);
    std::size_t q = n + combination - combination_for_line(p, n);
    if (p >= n || q >= n) {
        throw std::runtime_error("Undetermined ranks associated to a combination !");
    }
    ranks = {p, q};
}