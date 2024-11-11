#include <algorithm>
#include <cmath>

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
    return *this;
}

std::size_t SuperiorTriangularCombination::combination_for_line(const std::size_t& p) const { 
    return (p * (2 * ranks.size() - 1 - p)) / 2;
}

std::size_t SuperiorTriangularCombination::line_for_combination(const std::size_t& c) const {
    double b = 2 * ranks.size() - 1;
    double delta = b * b - 8 * c;
    return std::size_t(std::floor((b - std::sqrt(delta))*0.5));
}

std::size_t SuperiorTriangularCombination::operator()(const std::size_t& row, const std::size_t& column) const {
    std::size_t p = std::min(row, column);
    std::size_t q = std::max(row, column);
    return combination_for_line(p) + q - p - 1;
}

std::vector<std::size_t> SuperiorTriangularCombination::get_ranks(const std::size_t& combination) const {
    std::size_t p = line_for_combination(combination);
    std::size_t q = p + 1 + combination - combination_for_line(p);
    return {p, q};
}
