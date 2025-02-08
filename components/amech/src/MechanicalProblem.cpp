#include "GlobalTimer.h"
#include "MechanicalProblem.h"

using namespace amech;

void MechanicalProblem::init_output_resume() {
    std::string input_file = get_parser_value<std::string>("input_file");
    output_resume.init(input_file); 
}

void MechanicalProblem::read_input_data() {
    std::string app_path = get_parser_value<std::string>("application_path");
    std::string input_file = get_parser_value<std::string>("input_file");
    std::string input_commands = app_path + "/" +  get_parser_value<std::string>("input_commands");

    std::string msg = translate("READ_INPUT_DATA", input_file);
    output_resume.write(msg);

    input_data = std::make_shared<adata::DataManager>();
    input_data->read_data(input_file, input_commands);
    input_data->verify();
}

void MechanicalProblem::set_physical_data() {
    physical_data = std::make_shared<adata::Collections>();
}

void MechanicalProblem::init() {
    // Initialize the output resume file and folder
    init_output_resume();

    // Read the physical data from the ressources files
    set_physical_data();

    // Read the user input data
    start_timer("read_input_data");
    read_input_data();
    stop_timer("read_input_data");

    // Create Mechanical objects: transient, section, ...
    const adata::ProblemTransient transient = input_data->get_transient(0);
    std::cout << "Transient name: " << transient.name << std::endl;
}

void MechanicalProblem::verify() {

}

void MechanicalProblem::solve() {

}

void MechanicalProblem::close() {
    // Stop and write the timers    
    output_resume.write_timers();
}