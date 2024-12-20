#include "ProblemMaterial.h"

using namespace adata;

std::string ProblemMaterial::_get_correction_name(const std::string& int_correction) const {
    std::string correction = str::lowercase(int_correction);
    for (const auto& [key, value] : _correction_map) {
        if (key == correction || value == correction ) {
            return key;
        }
    }
    return "";
}

void ProblemMaterial::set_material_parameters(const std::shared_ptr<abase::BaseCommand>& command) {
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


void ProblemMaterial::init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t id) {
    // get the name of the material
    abase::get_child_value(command, "NAME", name);
    if (name.empty()) default_name(id);

    abase::get_child_value(command, "LAW", base_material_id);
    abase::get_child_values(command, "KETABLE", Ke_table_name);

    abase::get_child_value(command, "EREF", Eref);
    abase::get_child_value(command, "TYPM", typm);
    abase::get_child_value(command, "KF", Kf);
    abase::get_child_value(command, "TABLE", user_law_table);

    // Get the type of mean stress correction
    std::string read_correction;
    abase::get_child_value(command, "CORRECTION", read_correction);
    if (!read_correction.empty()) correction = _get_correction_name(read_correction);

    // Set coefficients parameters
    for (auto& name : command->get_children_names()) {
        auto sub_cmd = command->get_child(name);
        if (sub_cmd->get_type() == "coefficient") set_material_parameters(sub_cmd);
    }

}