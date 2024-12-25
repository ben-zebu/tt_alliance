#include <algorithm>

#include "ProblemPlate.h"

using namespace adata;

void plate_angle::init(double delta, double max, double min) {
    this->delta = delta;
    this->max = std::max(max, min);
    this->min = std::min(max, min);
    this->values.clear();
    if (delta > 0.) {
        for (double i = this->min; i <= this->max; i += delta) {
            this->values.push_back(i);
        }
    }
}


void ProblemPlate::init_user_coefficients(const std::shared_ptr<abase::BaseCommand>& command) {
    std::vector<double> angles;
    std::vector<double> coefs;

    abase::get_child_values(command, "ANGLE", angles);
    for (const auto& key : {"APHI", "BPHI", "CPHI"}) {
        coefs.clear();
        abase::get_child_values(command, key, coefs);
        if (angles.size() * coefs.size() > 0) {
            if (key == "APHI") user_coefficients.set_a_phi(angles, coefs);
            if (key == "BPHI") user_coefficients.set_b_phi(angles, coefs);
            if (key == "CPHI") user_coefficients.set_c_phi(angles, coefs);
        }
    }
}

void ProblemPlate::init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t category) {
    this->category = category;

    // get the type of the analysis : keywords SQUARE or TRIANGULAR can be followed by a subcase definition for 
    // category 2 analysis 
    // --> used of VectorStringCommand class to read potential values as subcase definition
    //
    std::size_t count_def = 0;
    // check square type
    std::vector<std::string> square_type = { "undefined" };
    abase::get_child_values(command, "SQUARE", square_type);
    if (square_type.empty() || square_type[0] != "undefined") {
        count_def++;
        type = "square";
        if (!square_type.empty())  sub_case = square_type[0];
    } 
    // check triangular type
    std::vector<std::string> triangular_type = { "undefined" };
    abase::get_child_values(command, "TRIANGULAR", triangular_type);
    if (triangular_type.empty() || triangular_type[0] != "undefined") {
        count_def++;
        type = "triangular";
        if (!triangular_type.empty())  sub_case = triangular_type[0];
    } 
    // mutual exclusion between square and triangular type
    if (count_def != 1) {
        input_error(translate("ERROR_PLATE_UNDEFINED_TYPE"));
    } 

    // check if the analysis is 2D
    std::string value = "";
    abase::get_child_value(command, "BIDIM", value);
    if (!value.empty())  if_2D = true;
    // check if the analysis is single angle
    value = "";
    abase::get_child_value(command, "PTSINGLE", value);
    if (!value.empty())  if_single_angle = true;
    // check if the analysis is 3Sm
    value = "";
    abase::get_child_value(command, "N3SM", value);
    if (!value.empty())  if_3Sm = false;

    // get angles parameters
    theta.max = 90.;
    abase::get_child_value(command, "DTHETA", theta.delta);
    abase::get_child_value(command, "TMAX", theta.max);
    abase::get_child_value(command, "TMIN", theta.min);
    phi.max = 180.;
    abase::get_child_value(command, "DPHI", phi.delta);
    abase::get_child_value(command, "PMAX", phi.max);
    abase::get_child_value(command, "PMIN", phi.min);

    // Set user coefficients
    std::size_t nang = 0;
    abase::get_child_value(command, "ANGLE", nang);
    if (nang > 0) init_user_coefficients(command->get_child("ANGLE"));

    // build intermediate angles values
    theta.init(theta.delta, theta.max, theta.min);
    phi.init(phi.delta, phi.max, phi.min);

    // For primary stress analysis, phi angles are remakable angles
    if (category != 2) {
        phi.values.clear();
        phi.values = get_3Sm_angles();
    }

}

void ProblemPlate::verify(const std::string& filecontext) const {
    // check if the subcase of the plate is defined
    if (category == 2 && sub_case.empty()) {
        std::string msg = translate("ERROR_PLATE_UNDEFINED_SUBCASE");
        file_input_error(msg, filecontext);
    }

    // For 2D analysis, theta values must be defined
    if (if_2D && theta.values.empty()) {
        std::string msg = translate("ERROR_EMPTY_THETA");
        file_input_error(msg, filecontext);
    }

    // For 3D analysis, theta values must be empty
    if (!if_2D && theta.values.size() > 0) {
        std::string msg = translate("ERROR_THETA_NOT_EMPTY");
        file_input_error(msg, filecontext);
    }

    // check coherence between phi and 3Sm angles
    if (category == 2 && if_3Sm) verify_phi(filecontext);

}

void ProblemPlate::verify_phi(const std::string& filecontext) const {
    for (const auto& v : get_3Sm_angles()) {
        auto it = std::find(phi.values.begin(), phi.values.end(), v);
        if (it == phi.values.end()) {
            std::string msg = translate("ERROR_MISMATCH_PHI_3SM", std::to_string(v));
            file_input_error(msg, filecontext);
        }
    }
}

std::vector<double> ProblemPlate::get_3Sm_angles() const {
    if (!if_3Sm) return {};
    if (type == "triangular") return {0., 60., 120.};
    if (type == "square") return {0., 45., 90., 135.};
    return {};
}