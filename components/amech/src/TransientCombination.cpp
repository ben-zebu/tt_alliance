#include "TransientCombination.h"

using namespace amech;

amath::TriangularCombination TransientCombination::transient_combination(std::size_t trk) const {
    std::vector<std::size_t> ranks = input_data->get_transient(trk).loadsteps;
    amath::TriangularCombination explorer = amath::TriangularCombination(ranks);
    explorer.set_type(false);
    return explorer;
}

amath::RectangularCombination   TransientCombination::crossed_combination(std::size_t trk1, std::size_t trk2) const {
    std::vector<std::size_t> ranks_1 = input_data->get_transient(trk1).loadsteps;
    std::vector<std::size_t> ranks_2 = input_data->get_transient(trk2).loadsteps;
    return amath::RectangularCombination(ranks_1, ranks_2);
}

amath::RectangularCombination TransientCombination::fictive_combination(std::size_t trk, std::size_t pivot_rank) const {
    std::vector<std::size_t> ranks_1 = input_data->get_transient(trk).loadsteps;
    std::vector<std::size_t> ranks_2 = {pivot_rank};
    return amath::RectangularCombination(ranks_1, ranks_2);
}

std::vector<std::string> TransientCombination::common_groups(std::size_t trk1, std::size_t trk2) const {
    std::vector<std::string> groups_1 = input_data->get_transient(trk1).groups;
    std::vector<std::string> groups_2 = input_data->get_transient(trk2).groups;
    std::vector<std::string> common_groups;
    std::set_intersection(groups_1.begin(), groups_1.end(), groups_2.begin(), groups_2.end(), 
                          std::back_inserter(common_groups));
    return common_groups;
}

std::vector<std::string> TransientCombination::crossing_transients(std::size_t trk1, std::size_t trk2) const {
    std::vector<std::string> crossing_transients;
    std::vector<std::string> groups_1 = input_data->get_transient(trk1).groups;
    std::vector<std::string> groups_2 = input_data->get_transient(trk2).groups;
    
    // no crossing transient are used if the transients have common groups
    if (common_groups(trk1, trk2).size() > 0) return crossing_transients;

    // search for transients defined as crossing transient and that links common groups of the 2 transients.
    for (std::size_t rk = 0; rk < input_data->nb_transients(); ++rk) {
        const auto& name = input_data->get_transient(rk).name;
        const auto& crossing_group = input_data->get_transient(rk).crossing_transient;;
        if (crossing_group.size() == 0) continue;

        std::size_t nb_grp1 = 0; std::size_t nb_grp2 = 0;
        for (const auto& grp : crossing_group) {
            if (std::find(groups_1.begin(), groups_1.end(), grp) != groups_1.end()) nb_grp1++;
            if (std::find(groups_2.begin(), groups_2.end(), grp) != groups_2.end()) nb_grp2++;
        }
        if (nb_grp1 == 1 && nb_grp2 == 0) crossing_transients.push_back(name);
    }
    return crossing_transients;
}

bool TransientCombination::is_shared_group(std::size_t trk1, std::size_t trk2) const {
    std::string shared_grp1 = input_data->get_transient(trk1).shared_group;
    std::string shared_grp2 = input_data->get_transient(trk2).shared_group;
    return shared_grp1 == shared_grp2;
}