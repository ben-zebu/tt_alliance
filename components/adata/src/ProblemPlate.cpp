#include <algorithm>

#include "ProblemPlate.h"

using namespace adata;

void plate_angle::init(double delta, double max, double min) {
    this->delta = delta;
    this->max = max;
    this->min = min;
    this->values.clear();
    for (double i = min; i <= max; i += delta) {
        this->values.push_back(i);
    }
}

void ProblemPlate::init(std::shared_ptr<abase::BaseCommand> command, std::size_t category) {
    this->category = category;

    // get the type of the analysis
    std::string square_type = "default";
    abase::get_child_value(command, "SQUARE", square_type);
    std::string triangular_type = "default";
    abase::get_child_value(command, "TRIANGULAR", triangular_type);
    if (square_type == "default" && triangular_type == "default") {
        input_error("ERROR_PLATE_UNDEFINED_TYPE");
    } else if (square_type != "default") {
        type = "square";
        if (!square_type.empty())  sub_case = square_type;
    } else {
        type = "triangular";
        if (!triangular_type.empty())  sub_case = triangular_type;
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

    // build intermediate angles values
    theta.init(theta.delta, theta.max, theta.min);
    phi.init(phi.delta, phi.max, phi.min);

    // For primary stress analysis, phi angles are remakable angles
    if (category != 2) {
        phi.values.clear();
        phi.values = get_3Sm_angles();
    }

}

void ProblemPlate::verify(std::string& filecontext) const {



    // For 2D analysis, theta values must be defined
    if (if_2D && theta.values.empty()) {
        std::string msg = translate("ERROR_EMPTY_THETA");
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