#include <algorithm>

#include "ProblemTransient.h"

using namespace adata;

std::size_t ProblemTransient::max_loadstesp() const {
    std::size_t max = 0;
    for (const auto& l : loadsteps) {
        if (l > max) max = l;
    }
    return max;
}

void ProblemTransient::init(std::shared_ptr<abase::BaseCommand> command, std::size_t id, std::size_t nb_loadsteps) {

    _nb_loadsteps = nb_loadsteps;

    // get the name of the transient
    abase::get_child_value(command, "NAME", name);
    if (name.empty()) default_name(id);

    // get the number of cycles and loadsteps
    abase::get_child_value(command, "NCY", nb_cycles);
    abase::get_child_values(command, "NUMBERS", loadsteps);
    if (!loadsteps.empty()) {
        for (std::size_t i = 0; i < loadsteps.size(); ++i) loadsteps[i] -= 1;
    }

    // get the groups and associated children parameters
    abase::get_child_values(command, "GROUP", groups);
    if (!groups.empty()) {
        is_alone = false;
        auto grp_cmd = command->get_child("GROUP");
        abase::get_child_values(grp_cmd, "GDF", crossing_transient);
        abase::get_child_value(grp_cmd, "GDP", shared_group);
        std::cout << "crossing_transient: " << crossing_transient.size() << std::endl;
    }

    // get the variant group and its number of cycles
    abase::get_child_value(command, "GVAR", variant_group);
    abase::get_child_value(command, "NVAR", variant_cycles);

    // get boolean flags
    std::string value = "";
    abase::get_child_value(command, "SINGLE", value);
    if (!value.empty())  is_alone = true;

    value = "";
    abase::get_child_value(command, "N3SM", value);
    if (!value.empty())  is_3sm = false;
    
    value = "";
    abase::get_child_value(command, "NROC", value);
    if (!value.empty())  is_thermal_ratchet = false;

}

void ProblemTransient::verify(std::string& filecontext) const {
    // check loadsteps indices
    if (max_loadstesp() >= _nb_loadsteps) {
        std::string max = std::to_string(max_loadstesp() + 1);
        std::string nb = std::to_string(_nb_loadsteps);
        std::string msg = translate("ERROR_OUT_OF_RANGE_LOADSTEPS", {max, nb});
        file_input_error(msg, filecontext);
    }

    // check groups and is_alone are mutually exclusive
    if (!groups.empty() && is_alone) {
        std::string msg = translate("ERROR_GROUP_ALONE");
        file_input_error(msg, filecontext);
    }

    // check in crossing transient is composed of 2 groups
    if (crossing_transient.size() > 0 && crossing_transient.size() != 2) {
        std::string msg = translate("ERROR_CROSSING_TRANSIENT");
        file_input_error(msg, filecontext);
    }

    // check in crossing transient definition match with groups definition
    if (crossing_transient.size() == 2) {
        auto start = std::find(groups.begin(), groups.end(), crossing_transient[0]);
        auto end = std::find(groups.begin(), groups.end(), crossing_transient[1]);
        if (start == groups.end() || end == groups.end()) {
            std::string ct0 = crossing_transient[0];
            std::string ct1 = crossing_transient[1];
            std::string msg = translate("ERROR_CROSSING_TRANSIENT_DEFINITION", {ct0, ct1});
            file_input_error(msg, filecontext);
        }
    }

    // check variant group and variant cycles are defined together or not
    if ( (variant_group.empty() && variant_cycles > 0) || (!variant_group.empty() && variant_cycles == 0) ) {
        std::string msg = translate("ERROR_VARIANT_DEFINITION");
        file_input_error(msg, filecontext);
    }

    // check if variant group and shared group are mutually exclusive
    if (!variant_group.empty() && !shared_group.empty()) {
        std::string msg = translate("ERROR_VARIANT_SHARED");
        file_input_error(msg, filecontext);
    }


}