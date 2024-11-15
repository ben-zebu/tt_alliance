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
    set_cached_size();
}

Combination::Combination(const Combination& combination) {
    this->ranks = combination.ranks;
    set_cached_size();
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
TriangularCombination& TriangularCombination::operator=(const TriangularCombination& combination) {
    if (this != &combination) {
        this->ranks = combination.ranks;
    }
    set_cached_size();
    return *this;
}

std::size_t TriangularCombination::combination_for_line(const std::size_t& p) const { 
    return (p * (2 * ranks.size() + 1 - p)) / 2;
}

std::size_t TriangularCombination::line_for_combination(const std::size_t& c) const {
    double b = 2 * ranks.size() + 1;
    double delta = b * b - 8 * c;
    return std::size_t(std::floor((b - std::sqrt(delta))*0.5));
}

std::size_t TriangularCombination::operator()(const std::size_t& row, const std::size_t& column) const {
    std::size_t p = std::min(row, column);
    std::size_t q = std::max(row, column);
    return combination_for_line(p) + q - p;
}

std::vector<std::size_t> TriangularCombination::get_ranks(const std::size_t& combination) const {
    std::size_t p = line_for_combination(combination);
    std::size_t q = p + combination - combination_for_line(p);
    return {p, q};
}

//
// SuperiorTriangularCombination methods
//
SuperiorTriangularCombination& SuperiorTriangularCombination::operator=(const SuperiorTriangularCombination& combination) {
    if (this != &combination) {
        this->ranks = combination.ranks;
    }
    set_cached_size();
    return *this;
}

constexpr std::size_t SuperiorTriangularCombination::combination_for_line(const std::size_t& p, const std::size_t n) const { 
    return (p + 1) * (2 * (n - 1) - p) / 2;
}

constexpr std::size_t SuperiorTriangularCombination::line_for_combination(const std::size_t& c, const std::size_t n) const {
    double alpha = 2 * n - 1;
    double delta = alpha * alpha - 4 * (2 * c - alpha);
    return std::size_t(std::floor((std::sqrt(delta) - alpha + 1)*0.5));
}

std::size_t SuperiorTriangularCombination::operator()(const std::size_t& row, const std::size_t& column) const {
    std::size_t p = std::min(row, column);
    std::size_t q = std::max(row, column);
    return combination_for_line(p, ranks.size()) + q - p - 1;
}

std::vector<std::size_t> SuperiorTriangularCombination::get_ranks(const std::size_t& combination) const {
    combi_ranks oranks;
    ranks_by_ptr(combination, oranks);
    std::vector<std::size_t> out({oranks.first, oranks.second});
    return out;
}

void SuperiorTriangularCombination::ranks_by_ptr(const std::size_t& combination, combi_ranks& oranks) const {
    std::size_t n = ranks.size();
    std::size_t p = line_for_combination(combination, n);
    std::size_t q = combination_for_line(p, n) -  combination;
    if (combination <= 499501) {
        double b = 2 * 1000 - 1;
        double delta = b * b - 8 * combination;
        double lc = std::floor((b - std::sqrt(delta))*0.5);
        std::cout << "combination = " << combination << std::endl;
        std::cout << "   cached_size = " << cached_size << std::endl;
        std::cout << "   b = " << b << ", delta = " << delta << ", lc = " << lc << std::endl;
        std::cout << "   line_for_combination = " << line_for_combination(combination, n) << std::endl;
        std::cout << "   combination_for_line = " << combination_for_line(p, n) << std::endl;
        std::cout << "   => p = " << p << ", q = " << q << std::endl;
        throw std::runtime_error("STOP");
    }

    oranks = {p, q};
}