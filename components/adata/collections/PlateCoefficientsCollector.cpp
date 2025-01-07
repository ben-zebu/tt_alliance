#include "PlateCoefficientsCollector.h"

using namespace adata;

void PlateCoefficientsCollector::add_coefficient(std::shared_ptr<PlateCoefficients> coef, const std::string& filecontext) {
    if (get_coefficient(coef->function_id, coef->Ph) != nullptr) {
        std::string msg = translate("PLATE_FUNCTION_CONFLICT", {coef->function_id, std::to_string(coef->Ph)});
        file_input_error(msg, filecontext);
    }    
    coefficients_data.push_back(coef);
}

void PlateCoefficientsCollector::set_data(const std::shared_ptr<abase::BaseCommand>& command, 
                                          const std::string& filecontext) {

    std::string function_id, description;
    std::vector<double> ph_values;
    std::vector<double> angles_values, a_phi_values, b_phi_values, c_phi_values;

    // get data from the command
    abase::get_child_value(command, "FUNCTION_ID", function_id);
    abase::get_child_value(command, "DESCRIPTION", description);
    abase::get_child_values(command, "PH", ph_values);
    abase::get_child_values(command, "ANGLE", angles_values);
    abase::get_child_values(command, "APHI", a_phi_values);
    abase::get_child_values(command, "BPHI", b_phi_values);
    abase::get_child_values(command, "CPHI", c_phi_values);

    std::size_t ph_sz = ph_values.size();
    std::size_t angles_sz = angles_values.size();
    std::size_t coef_sz = ph_sz * angles_sz;
    if (coef_sz != a_phi_values.size() || coef_sz != b_phi_values.size() || coef_sz != c_phi_values.size()) {
        std::string msg = translate("PLATE_FUNCTION_SIZE", {function_id, coef_sz});
        file_input_error(msg, filecontext);
    }
    
    for (std::size_t i = 0; i < ph_values.size(); ++i) {
        std::shared_ptr<PlateCoefficients> coef = std::make_shared<PlateCoefficients>();

        std::size_t start = i * angles_sz;
        std::size_t end = start + angles_sz;

        coef->function_id = function_id;
        coef->description = description;
        coef->Ph = ph_values[i];
        coef->angles = angles_values;
        coef->a_phi = std::vector<double>(a_phi_values.begin() + start, a_phi_values.begin() + end);
        coef->b_phi = std::vector<double>(b_phi_values.begin() + start, b_phi_values.begin() + end);
        coef->c_phi = std::vector<double>(c_phi_values.begin() + start, c_phi_values.begin() + end);

        add_coefficient(coef, filecontext);
    }
}

std::shared_ptr<PlateCoefficients> PlateCoefficientsCollector::get_coefficient(const std::string& function_id, 
                                                                               const double ph) const {
    for (const auto& coef : coefficients_data) {
        if (coef->function_id == function_id && coef->Ph == ph) return coef;
    }
    return nullptr;
}

std::pair<double, double> PlateCoefficientsCollector::get_ph_range(const std::string& function_id, 
                                                                   const double ph) const {
    double lower_ph = std::numeric_limits<double>::min();
    double upper_ph = std::numeric_limits<double>::max();

    for (const auto& coef : coefficients_data) {
        // Check if the function_id is the same
        if (coef->function_id != function_id) continue;

        // build p/h bounds
        if (coef->Ph < ph) lower_ph = std::max(lower_ph, coef->Ph);
        if (coef->Ph > ph) upper_ph = std::min(upper_ph, coef->Ph);
    }

    return std::make_pair(lower_ph, upper_ph);
}