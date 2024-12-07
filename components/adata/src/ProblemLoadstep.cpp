#include "ProblemLoadstep.h"

using namespace adata;

void ProblemLoadstep::init(std::shared_ptr<abase::BaseCommand> command, std::size_t nb_torsors) {
    _nb_torsors = nb_torsors;
    command->get_values(pressure_cards);

    abase::get_child_values(command, "KP", pressure_values);
    abase::get_child_value(command, "THERMAL", thermal_card);
    if (thermal_card > 0) {
        auto subcommand = command->get_child("THERMAL");
        auto khcommand = subcommand->get_child("KH");
        if (khcommand) khcommand->get_value(kh);
    }

    // coefficients associated to external torsors without earthquake
    auto efcommand = command->get_child("EF");
    if (efcommand) {
        std::vector<double> values;
        efcommand->get_values(values);
        if (values.size() == nb_torsors) {
            max_ef = values;
            min_ef = values;
        } 
        else if (values.size() == 2*nb_torsors) {
            max_ef = std::vector<double>(values.begin(), values.begin() + nb_torsors);
            min_ef = std::vector<double>(values.begin() + nb_torsors, values.end());
        } else {
            if (values.size() > 0) _mismatch_coefficients = true;
        }
    }

    // coefficients associated to external torsors with earthquake
    auto ftcommand = command->get_child("FT");
    if (ftcommand) {
        std::vector<double> values;
        ftcommand->get_values(values);
        if (values.size() == nb_torsors) {
            max_ft = values;
            min_ft = values;
        } 
        else if (values.size() == 2*nb_torsors) {
            max_ft = std::vector<double>(values.begin(), values.begin() + nb_torsors);
            min_ft = std::vector<double>(values.begin() + nb_torsors, values.end());
        } else {
            if (values.size() > 0) _mismatch_coefficients = true;
        }
    }
}

void ProblemLoadstep::verify(std::string& filecontext) const {
    // check if the number of pressure cards is equal to the number of pressure values
    if (pressure_cards.size() != pressure_values.size()) {
        std::string nb_cards = std::to_string(pressure_cards.size());
        std::string nb_values = std::to_string(pressure_values.size());
        std::string msg = translate("PRESSURE_CARDS_VALUES_MISMATCH", {nb_cards, nb_values});
        file_input_error(msg, filecontext);
    }
    
    // check if coefficients associated to external torsors are correctly initialized
    if (_mismatch_coefficients) {
        std::string s_nb = std::to_string(_nb_torsors);
        std::string d_nb = std::to_string(2*_nb_torsors);
        std::string msg = translate("MISMATCH_COEFFICIENTS", {s_nb, d_nb});
        file_input_error(msg, filecontext);
    }
}