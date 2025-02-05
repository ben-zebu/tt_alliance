#include "TransientCombination.h"

using namespace amech;

amath::TriangularCombination TransientCombination::transient_combination(const std::string& transient) const {
    std::vector<std::size_t> ranks = input_data->get_transient(transient).loadsteps;
    amath::TriangularCombination explorer = amath::TriangularCombination(ranks);
    explorer.set_type(false);
    return explorer;
}

amath::RectangularCombination   TransientCombination::crossed_combination(const std::string& first_transient, 
                                                                          const std::string& second_transient) const {
    std::vector<std::size_t> ranks_1 = input_data->get_transient(first_transient).loadsteps;
    std::vector<std::size_t> ranks_2 = input_data->get_transient(second_transient).loadsteps;
    return amath::RectangularCombination(ranks_1, ranks_2);
}

amath::RectangularCombination TransientCombination::fictive_combination(const std::string& transient, std::size_t pivot_rank) const {
    std::vector<std::size_t> ranks_1 = input_data->get_transient(transient).loadsteps;
    std::vector<std::size_t> ranks_2 = {pivot_rank};
    return amath::RectangularCombination(ranks_1, ranks_2);
}