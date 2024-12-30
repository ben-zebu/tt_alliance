#include "ProblemSection.h"

using namespace adata::parts;

void ProblemSection::set_material_parameters(const std::shared_ptr<abase::BaseCommand>& command) {
    // get the name of the coefficient
    std::string coef_name = command->get_name();

    // Fix value case for coefficient
    std::vector<double> values;
    double value = 0.;
    command->get_values(values);
    if (values.size() == 1) value = values[0];

    // reference to a table
    std::string table_name = "";
    command->get_value(table_name);

    material_parameters_input[coef_name] = {table_name, value};
}

void ProblemSection::init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t id) {
    // Set the internal name of the section
    default_name(id);
    // get the fem rank of the section
    std::vector<std::size_t> uint_values;
    command->get_values(uint_values);
    if (uint_values.size() == 1) fem_rank = uint_values[0];
    // get the fem name of the section
    abase::get_child_value(command, "NAME", name);
    
    // Set material id for each skin of the section
    std::vector<std::string> str_values;
    abase::get_child_values(command, "MATERIAL", str_values);
    if (str_values.size() == 1) {
        material_id["inner"] = str_values[0];
        material_id["outer"] = str_values[0];
    } else if (str_values.size() == 2) {
        material_id["inner"] = str_values[0];
        material_id["outer"] = str_values[1];
    }

    // Get the fatigue law id
    abase::get_child_value(command, "LAW", fatigue_law_id);

    // Get the type of temperature dependence for Ec/E calculation
    for (const auto& type : {"none", "mean", "min", "max"}) {
        std::string key = "T" + str::uppercase(type);
        std::string str_value = "";
        abase::get_child_value(command, key, str_value);
        if (!str_value.empty()) EcE_type = type;
    }
    
    // Set coefficients parameters
    for (auto& name : command->get_children_names()) {
        auto sub_cmd = command->get_child(name);
        if (sub_cmd->get_type() == "coefficient") set_material_parameters(sub_cmd);
    }

    std::string str_value = "";
    abase::get_child_value(command, "DRILLED", str_value);
    if (!str_value.empty()) is_drilled = true;

    str_value = "";
    abase::get_child_value(command, "NEUBER", str_value);
    if (!str_value.empty()) neuber_correction = true;

    str_value = "";
    abase::get_child_value(command, "MISES", str_value);
    if (!str_value.empty()) mises_criterion = true;


}