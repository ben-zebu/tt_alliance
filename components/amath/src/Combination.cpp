#include <algorithm>
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
// SquareCombination methods
//
SquareCombination& SquareCombination::operator=(const SquareCombination& combination) {
    if (this != &combination) {
        this->ranks = combination.ranks;
    }
    set_cached_size();
    return *this;
}

std::size_t SquareCombination::operator()(const std::size_t& row, const std::size_t& column) const {
    return row * ranks.size() + column;
}

std::vector<std::size_t> SquareCombination::get_ranks(const std::size_t& combination) const {
    return {combination / ranks.size(), combination % ranks.size()};
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

std::vector<std::size_t> TriangularCombination::get_ranks(const std::size_t& combination) const {
    combi_ranks oranks;
    ranks_by_ptr(combination, oranks);
    std::vector<std::size_t> out({oranks.first, oranks.second});
    return out;
}

void TriangularCombination::ranks_by_ptr(const std::size_t& combination, combi_ranks& oranks) const {
    std::size_t n = ranks.size();
    std::size_t p = line_for_combination(combination, n);
    std::size_t q = n + combination - combination_for_line(p, n);
    //std::cout << "p: " << p << " q: " << q << std::endl;
    oranks = {p, q};
}