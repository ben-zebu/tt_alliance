#include "PlateCoefficients.h"

using namespace adata::parts;

void PlateCoefficients::clear() {
    Ph = 0.;
    angles.clear();
    a_phi.clear();
    b_phi.clear();
    c_phi.clear();
}

void PlateCoefficients::init(const std::shared_ptr<abase::BaseCommand>& command) {
    clear();

    if (command->get_name() == "PH") command->get_value(Ph);
    abase::get_child_values(command, "ANGLE", angles);
    abase::get_child_values(command, "APHI", a_phi);
    abase::get_child_values(command, "BPHI", b_phi);
    abase::get_child_values(command, "CPHI", c_phi);
}

void PlateCoefficients::verify(const std::string& filecontext) const {
    std::string coef_msg = "";
    if (angles.size() != a_phi.size()) {
        coef_msg = coef_msg.empty() ? "a_phi" : ", a_phi";
    }
    if (angles.size() != b_phi.size()) {
        coef_msg = coef_msg.empty() ? "b_phi" : ", b_phi";
    }
    if (angles.size() != c_phi.size()) {
        coef_msg = coef_msg.empty() ? "c_phi" : ", c_phi";
    }
    
    if (coef_msg.size() > 0) {
        std::string str_nang = std::to_string(angles.size());
        std::string msg = translate("ERROR_USER_COEFFICIENTS_SIZE", {coef_msg, str_nang});
        file_input_error(msg, filecontext);
    }
}