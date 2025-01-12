#include "Collections.h"

using namespace adata;

void Collections::fill(abase::DataCollector& collector, const std::string& conf_files, const std::string& conf_commands) {
    // get the application path
    std::string app_path = get_parser_value<std::string>("application_path");

    // get the commands associated to the collector
    std::string commands = app_path + "/" +  get_parser_value<std::string>(conf_commands);

    // get the files that store all data
    std::string str_files = get_parser_value<std::string>(conf_files);
    std::vector<std::string> files = str::split(str::replace(str_files, ",", " "));
    
    // read data from the files
    for (const auto& file : files) {
        collector.read_data(app_path + "/" + file, commands);
    }

}

Collections::Collections() {
    fill(materials, "material_files", "material_commands");
    fill(laws, "fatigue_law_files", "material_commands");
    fill(plate_coefficients, "plate_files", "plate_commands");
}

std::shared_ptr<BaseMaterial> Collections::get_material(const std::string& material_id) const {
    return materials.get_material(material_id);
}

std::shared_ptr<FatigueLaw> Collections::get_law(const std::string& law_id, 
                                                 const std::string& code, const std::string& edition) const {
    return laws.get_law(law_id, code, edition);
}

std::shared_ptr<PlateCoefficients> Collections::get_coefficient(const std::string& function_id, const double ph) const {
    return plate_coefficients.get_coefficient(function_id, ph);
}

std::pair<double, double> Collections::get_ph_range(const std::string& function_id, const double ph) const {
    return plate_coefficients.get_ph_range(function_id, ph);
}