#include "ProblemSectionCoefficient.h"

using namespace adata::parts;

void StressCoefficient::set_default_values() {
    if (values.empty()) values.resize(expected_size, 1.);
}

void StressCoefficient::init(const std::shared_ptr<abase::BaseCommand>& command) {
    abase::get_child_values(command, name, values);
    set_default_values();
}

void StressCoefficient::verify(const std::string& filecontext) const {
    if (values.size() != expected_size) {
        std::string msg = translate("ERROR_SECTION_COEFFICIENTS", {name, std::to_string(expected_size)});
        file_input_error(filecontext, msg);
    }
}

void OvalCoefficient::set_other_values() {
    if (values.size() == 1) values.resize(expected_size, values[0]);
    if (values.size() == 2) {
        double Slin = values[0];
        double Stot = values[1];
        values.resize(expected_size);
        values[0] = values[4] = Stot;
        values[1] = values[2] = values[3] = Slin;
    }
}

void OvalCoefficient::init(const std::shared_ptr<abase::BaseCommand>& command) {
    StressCoefficient::init(command);
    set_other_values();
}
