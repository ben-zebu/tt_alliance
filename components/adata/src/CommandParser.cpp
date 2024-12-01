#include <iostream>
#include <string>

#include "ConfigParser.h"
#include "CommandParser.h"
#include "Environment.h"
#include "LocalPath.h"

using namespace adata;

CommandParser::CommandParser(int argc, char** argv) {
    // set the path of the executable for future purpose
    set_application_path(argv[0]);

    int arg_rk = 1;
    while (arg_rk < argc) {
        std::string name = argv[arg_rk];
        std::string value = (arg_rk + 1 < argc) ? argv[arg_rk + 1] : "";

        if (name == "--version") version();
        if (name == "--help") help();

        if (name == "--language") {
            set_parser_value("language", value);
            arg_rk +=2;
            continue;
        } 
        
        bool is_integer_option = false;
        for (const auto& key : integer_options) {
            if (key != name) continue;
            set_integer_option(key, value);
            is_integer_option = true;
            arg_rk += 2;
        }
        if (is_integer_option) continue;

        bool is_boolean_option = false;
        for (const auto& key : boolean_options) {
            if (key != name) continue;
            set_boolean_option(key);
            is_boolean_option = true;
            arg_rk += 1;
        }
        if (is_boolean_option) continue;

        if (name.find_first_of("-") == 0) {
            input_error(translate("UNKNOWN_OPTION", {name, value}));
        }

        // standard text for input file    
        set_parser_value("input_file", name);            
        arg_rk += 1;
    }

}

void CommandParser::set_application_path(const std::string& path) {
    std::string app_path = abase::getAppPath(path);
    set_parser_value("application_path", app_path);
}

void CommandParser::set_integer_option(const std::string& key, const std::string& value) const {
    int ivalue = 0;
    try {
        ivalue = std::stoi(value);
    } catch (const std::exception& e) {
        input_error(translate("OPTION_INTEGER_CONVERT", {key, value}));
    }
    set_parser_value(key, value);
}

void CommandParser::set_boolean_option(const std::string& key) const {
    set_integer_option(key, "1");
}

void CommandParser::help() {
    std::exit(0);
}
    
void CommandParser::version() {
    std::string version = get_parser_value<std::string>("version");
    std::cout << "Alliance version " << version << std::endl;
    std::exit(0);
}
