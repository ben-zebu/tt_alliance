#include <algorithm>
#include <cctype>
#include <sstream>
#include <cassert>

#include "CommandsString.h"
#include "CommandsCollector.h"
#include "Environment.h"

using namespace abase;

std::size_t StringCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    _value = reader.get_word();
    if (_value.empty()) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_MISSING_PARAMETER", key), filecontext);
    }
    _read_status = true;
    reader.move();

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}

std::size_t VectorStringCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    std::string str_value = reader.get_word();
    // Read the values until a command name is found.
    while (!str_value.empty() && !collector.is_command_name(str_value)) {
        _values.push_back(str_value);
        _read_status = true;

        reader.move();
        str_value = reader.get_word();
    }

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}

std::size_t MixStringCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    // Read the number of expected values
    std::string str_value = reader.get_word();
    std::stringstream ss(str_value);
    ss >> _n_values;
    if (ss.fail() || !ss.eof()) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_TYPE_CONVERSION", {key, str_value}), filecontext);
    }
    _read_status = true;
    reader.move();

    // Read the values with the expected type
    for (std::size_t i = 0; i < _n_values; i++) {
        str_value = reader.get_word();
        if (str_value.empty() || collector.is_command_name(str_value)) {
            std::string filecontext = reader.context_error();
            file_input_error(translate("ERROR_NB_VALUES", {key, std::to_string(_n_values)}), filecontext);
        }
        _values.push_back(str_value);
        _read_status = true;
        reader.move();
    }
    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}
