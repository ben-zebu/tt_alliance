#include "MechanicalProblem.h"

using namespace amech;

void MechanicalProblem::read_input_data() {
    std::string app_path = get_parser_value<std::string>("application_path");
    std::string input_file = get_parser_value<std::string>("input_file");
    std::string input_commands = app_path + "/" +  get_parser_value<std::string>("input_commands");

    input_data = std::make_shared<adata::DataManager>();
    input_data->read_data(input_file, input_commands);
    input_data->verify();
}

void MechanicalProblem::set_physical_data() {
    physical_data = std::make_shared<adata::Collections>();
}

void MechanicalProblem::init() {
    // Read the physical data from the ressources files
    set_physical_data();

    // Read the user input data
    read_input_data();
}
