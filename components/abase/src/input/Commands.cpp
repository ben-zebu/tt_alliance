#include <algorithm>
#include <cctype>
#include <sstream>
#include <cassert>

#include "Commands.h"
#include "CommandsCollector.h"
#include "Environment.h"

using namespace abase;

bool BaseCommand::is_same_keyword(const std::string& a_key) {
    // case for empty string
    if (a_key.empty()) return false;

    std::string lower_key = str::lowercase(a_key);
    for (const auto& translation : translations) {
        const auto& keys = translation.second;
        if (keys.empty()) continue;
        if (keys.size() == 1 && keys[0] == lower_key) return true;

        if (lower_key.size() < keys[0].size()) continue;

        std::size_t min_sz = std::min(keys[1].size(), lower_key.size());
        if (lower_key.substr(0, min_sz) == keys[1].substr(0, min_sz)) return true;
    }
    return false;
}

bool CompositeCommand::is_command_name(const std::string& name) {
    if (this->is_same_keyword(name)) return true;
    for (auto& child : _children) {
        if (child->is_command_name(name)) return true;
    }
    return false;
}

std::size_t CompositeCommand::children_process(FileReader& reader, const CommandsCollector& collector) {
    std::size_t status = 0;
    while(status == 0 && reader.get_word().size() > 0) {
        status = 1;
        for (auto& child : _children) {
            status *= child->read_input(reader, collector);
        }
    }
    return status;
}

void CompositeCommand::addChild(std::shared_ptr<BaseCommand> child) {
    _children.push_back(child);
}

std::shared_ptr<BaseCommand> CompositeCommand::get_child(const std::string& name) {
    for (auto& child : _children) {
        if (child->get_name() == name) return child;
    }
    return nullptr;
}

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

template<typename T>
std::pair<T, bool> NumericCommand<T>::convert_value(const std::string& key, FileReader& reader) {
    T output_value;
    bool status = true;
    std::string str_value = reader.get_word();
    std::string delimiters = STRING_DELIMITER;

    // Replace the string delimiters with a special string to avoid issue with stringstream.
    for (std::size_t i = 0; i < delimiters.size(); i++) {
        str_value = str::replace(str_value, delimiters.substr(i, 1), "&");
    }

    // Convert the string to the expected type.
    std::stringstream ss(str_value);
    ss >> output_value;

    if (ss.fail() || !ss.eof()) status = false;
    return {output_value , status};
}

template<typename T>
std::size_t ValueCommand<T>::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    std::pair<T, bool> conversion = this->convert_value(key, reader);
    if (!conversion.second) {
        std::string filecontext = reader.context_error();
        std::string str_value = reader.get_word();
        file_input_error(translate("ERROR_TYPE_CONVERSION", {key, str_value}), filecontext);
    }

    _value = conversion.first;
    this->_read_status = true;
    reader.move();

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}

template<typename T>
std::size_t VectorCommand<T>::read_input(FileReader& reader, const CommandsCollector& collector) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    clear();
    reader.move();

    std::string str_value = reader.get_word();
    // Read the values until the expected type conversion fails.
    while (str_value.size() > 0) {
        std::pair<T, bool> conversion = this->convert_value(key, reader);
        if (!conversion.second) break;
        _values.push_back(conversion.first);
        this->_read_status = true;

        reader.move();
        str_value = reader.get_word();
    }

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}

template<typename T>
std::size_t MixCommand<T>::read_input(FileReader& reader, const CommandsCollector& collector) {
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
    reader.move();

    // Read the values with the expected type
    for (std::size_t i = 0; i < _n_values; i++) {
        std::pair<T, bool> conversion = this->convert_value(key, reader);
        if (!conversion.second) {
            str_value = reader.get_word();
            std::string filecontext = reader.context_error();
            file_input_error(translate("ERROR_NB_VALUES", {key, std::to_string(_n_values)}), filecontext);
        }
        _values.push_back(conversion.first);
        this->_read_status = true;
        reader.move();
    }

    std::size_t children_status = this->children_process(reader, collector);
    return 0;
}


// Explicit template instantiation
template class NumericCommand<int>;
template class NumericCommand<double>;
template class NumericCommand<std::size_t>;

template class ValueCommand<int>;
template class ValueCommand<double>;
template class ValueCommand<std::size_t>;

template class VectorCommand<int>;
template class VectorCommand<double>;
template class VectorCommand<std::size_t>;

template class MixCommand<int>;
template class MixCommand<double>;
template class MixCommand<std::size_t>;



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
        _table_name = str_values[0];
        convert(std::vector<std::string>(str_values.begin() + 1, str_values.end()), status);
    } else {
        // try a conversion ignoring the potential table name
        convert(str_values, status);
        // if failed and impaire number of values, try to extract the table name
        if (!status && str_values.size() % 2 != 0) {
            status = true;
            _table_name = str_values[0];
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

