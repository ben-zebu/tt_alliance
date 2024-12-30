#include <algorithm>
#include <cctype>
#include <sstream>
#include <cassert>

#include "CommandsSpecial.h"
#include "CommandsCollector.h"
#include "Environment.h"

using namespace abase;

std::size_t SingleCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!is_same_keyword(key)) return 1;
    clear();
    _value = str::uppercase(key);
    _read_status = true;

    reader.move();
    std::size_t children_status = children_process(reader, collector);
    return 0;
}

std::vector<std::size_t> TimeStepCommand::convert_range(const std::string& start, const std::string& end,
                                                        const std::string& step, bool& status) {
    std::vector<std::size_t> range;
    try {
        std::size_t start_value = std::stoull(start);
        std::size_t end_value = std::stoull(end);
        std::size_t step_value = std::stoull(step);

        if (step_value == 0) {
            status = false;
            return range;
        }

        for (std::size_t value = start_value; value <= end_value; value += step_value) {
            range.push_back(value);
        }
        status = true;
    } catch (const std::invalid_argument& e) {
        status = false;
    } catch (const std::out_of_range& e) {
        status = false;
    }
    return range;
}

std::vector<std::size_t> TimeStepCommand::split_sequence(const std::string& sequence, bool& status) {
    std::vector<std::size_t> timesteps;

    // Replace the keywords by special characters and split it.
    std::string s = str::trim(sequence);
    for (auto& key : { "PAS", "STEP"}) s = str::replace(s, key, " % ");
    for (auto& key : { "A", "TO"}) s = str::replace(s, key, " : ");
    std::vector<std::string> str_values = str::split(s);

    // Convert the string values to unsigned integers.
    std::string start, end, step;
    std::size_t pos = 0;
    while (pos < str_values.size()) {
        s = str_values[pos];
        if (s == "%" || s == ":") {
            status = false;
            return timesteps;
        }

        // case for single timestep
        start = s;
        end = s;
        step = "1";
        pos++;

        // case for range of timesteps
        if (pos < str_values.size() - 1 && str_values[pos] == ":") {
            end = str_values[pos + 1];
            pos += 2;
            if (pos < str_values.size() - 1 && str_values[pos] == "%") {
                step = str_values[pos + 1];
                pos += 2;
            }
        }

        // Convert the string values to unsigned integers.
        std::vector<std::size_t> range = convert_range(start, end, step, status);
        if (!status) return timesteps;
        timesteps.insert(timesteps.end(), range.begin(), range.end());
    }

    return timesteps;
}

std::size_t TimeStepCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    std::string str_value = reader.get_word();
    std::string sequence = "";
    // Read the values until a command name is found.
    while (!str_value.empty() && !collector.is_command_name(str_value)) {
        sequence += str_value + " ";
        reader.move();
        str_value = reader.get_word();
    }

    if (sequence.empty()) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_UNREADABLE_TIMESTEP"), filecontext);
    }

    // convert the sequence of timesteps into a vector of timesteps
    bool status = true;
    _values = split_sequence(sequence, status);
    if (!status) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_UNREADABLE_TIMESTEP"), filecontext);
    }
    _read_status = true;

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}

void TableCommand::convert(const std::vector<std::string>& values, bool& status) {
    for (const auto& value : values) {
        try {
            _values.push_back( std::stod(value) );
        } catch (const std::invalid_argument& e) {
            status = false;
            return;
        } catch (const std::out_of_range& e) {
            status = false;
            return;
        }
    }
}

void TableCommand::split_sequence(const std::string& sequence, bool& status) {
    std::vector<std::string> str_values = str::split(sequence);

    bool is_name = (str_values[0].find("/") != std::string::npos);
    if (is_name) {
        _table_name = str_values[0].substr(0, str_values[0].find("/"));
        convert(std::vector<std::string>(str_values.begin() + 1, str_values.end()), status);
    } else {
        // try a conversion ignoring the potential table name
        convert(str_values, status);
        // if failed and impaire number of values, try to extract the table name
        if (!status && str_values.size() % 2 != 0) {
            status = true;
            _table_name = str_values[0].substr(0, str_values[0].find("/"));
            convert(std::vector<std::string>(str_values.begin() + 1, str_values.end()), status);
        }
    }

    // check if the number of values is pair
    if (_values.size() % 2 != 0) status = false;
}

std::size_t TableCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    // A table is defined by a name (optional) and a list of abciss and ordinates values.
    std::string str_value = reader.get_word();
    std::string sequence = "";
    while (!str_value.empty() && !collector.is_command_name(str_value)) {
        sequence += str_value + " ";
        reader.move();
        str_value = reader.get_word();
    }

    if (sequence.empty()) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_UNREADABLE_TABLE"), filecontext);
    }

    // collapse space around '/' character for name extraction
    sequence = str::replace(sequence, " /", "/");
    sequence = str::replace(sequence, "/ ", "/");

    bool status = true;
    split_sequence(sequence, status);

    if (!status) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_TABLE_CONVERSION"), filecontext);
    }
    _read_status = true;

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}

std::size_t CoefficientCommand::convert(const std::vector<std::string>& values, bool& status) {
    std::size_t pos = 0;
    for (const auto& value : values) {
        pos++;
        if (value == "TABLE") break;
        try {
            _values.push_back( std::stod(value) );
        } catch (const std::invalid_argument& e) {
            status = false;
            return pos;
        } catch (const std::out_of_range& e) {
            status = false;
            return pos;
        }
    }
    return pos;
}

void CoefficientCommand::split_sequence(const std::string& sequence, const std::string& key, 
                                        const std::string& filecontext) {
    std::vector<std::string> str_values = str::split(sequence);
    bool status = true;
    std::size_t table_pos = convert(str_values, status);

    // Set the table name if it exists
    if (table_pos < str_values.size()) _table_name = str_values[table_pos];
    
    // Only 1 or 0 value are expected
    if (_values.size() > 1) {
        std::string msg = translate("ERROR_COEFFICIENT_SIZE", {key, std::to_string(_values.size())} );
        file_input_error(msg, filecontext);
    }

    // check if the coefficient is negative for table name definition
    if (_values.size() == 1 && _values[0] < 0) {
        if (!_table_name.empty()) {
            std::string msg = translate("ERROR_COEFFICIENT_NAME", {key, _table_name, std::to_string(_values[0])});
            file_input_error(msg, filecontext);
        }
        _table_name = std::abs(_values[0]);
    }

    if (_values.size() == 0 && _table_name.empty()) {
        file_input_error(translate("ERROR_UNREADABLE_COEFFICIENT", key), filecontext);
    }

}

std::size_t CoefficientCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    // A table is defined by a name (optional) and a list of abciss and ordinates values.
    std::string str_value = reader.get_word();
    std::string sequence = "";
    while (!str_value.empty()) {
        std::string cmd_name = collector.get_command_name_by_keyword(str_value);
        if (collector.is_command_name(str_value) && cmd_name != "TABLE") break;

        if (cmd_name.empty()) {
            sequence += str_value + " ";
        } else {
            sequence += cmd_name + " ";
        }
        reader.move();
        str_value = reader.get_word();
    }

    std::string filecontext = reader.context_error();
    if (sequence.empty()) file_input_error(translate("ERROR_UNREADABLE_COEFFICIENT", key), filecontext);
    split_sequence(sequence, key, filecontext);

    _read_status = true;

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}

std::size_t FileCommand::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    // extract string sequence after the keyword in order to build the file name
    std::string str_value = reader.get_word();
    std::string line = reader.get_line();
    std::size_t pos = line.find(str_value);

    _value = line.substr(pos);
    if (_value.empty()) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_FILE_EXTRACTION"), filecontext);
    }

    reader.move_line();
    _read_status = true;
    return 0;
}
