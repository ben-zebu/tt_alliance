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
}

void MechanicalProblem::verify() {

}

void MechanicalProblem::solve() {

}

void MechanicalProblem::close() {
    // stop all timers and get the timing information
    stop_all_timers();
    std::unordered_map<std::string, std::pair<std::string,std::string>> all_times = get_all_timers();

    // print the global timer information
    std::string msg = translate("TIME_SUMMARY", {all_times["global_timer"].first, all_times["global_timer"].second});
    output_resume.write("\n" + msg);

    // print all timers information
    msg = "";
    for (const auto& timer : all_times) {
        if (timer.first == "global_timer") continue;
        if (msg.size() > 0) msg += "\n";
        msg += translate("TIME_METRICS", {timer.first, timer.second.first, timer.second.second});
    }
    if (msg.size() > 0) {
        msg = "\n" + translate("INTERNAL_TIME_METRICS") + "\n" + msg;
        output_resume.write(msg);
    }


    // double test_val = 10000000;
    // std::cout << "Test value: " << str::to_string(test_val, 4) << std::endl;
    // std::cout << "Test value: " << str::to_string_exp(test_val, 4) << std::endl;

}