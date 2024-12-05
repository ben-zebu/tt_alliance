#include <algorithm>
#include <cctype>
#include <sstream>
#include <cassert>

#include "Commands.h"
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

std::size_t CompositeCommand::children_process(FileReader& reader) {
    std::size_t status = 0;
    while(status == 0 && reader.get_word().size() > 0) {
        status = 1;
        for (auto& child : _children) {
            status *= child->read_input(reader);
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

std::size_t SingleCommand::read_input(FileReader& reader) {
    if (!is_same_keyword(reader.get_word())) return 1;
    reader.move();
    std::size_t children_status = children_process(reader);
    return 0;
}

std::size_t StringCommand::read_input(FileReader& reader) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    reader.move();

    _value = reader.get_word();
    if (_value.empty()) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_MISSING_PARAMETER", key), filecontext);
    }
    reader.move();

    std::size_t children_status = this->children_process(reader);
    return 0;
}

std::size_t VectorStringCommand::read_input(FileReader& reader) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    reader.move();

    std::string str_value = reader.get_word();
    // Read the values until a command name is found.
    while (!str_value.empty() && !this->is_command_name(str_value)) {
        _values.push_back(str_value);
        
        reader.move();
        str_value = reader.get_word();
    }
    
    std::size_t children_status = this->children_process(reader);
    return 0;
}

std::size_t MixStringCommand::read_input(FileReader& reader) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
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
        str_value = reader.get_word();
        if (str_value.empty() && this->is_command_name(str_value)) {
            std::string filecontext = reader.context_error();
            file_input_error(translate("ERROR_NB_VALUES", {key, std::to_string(_n_values)}), filecontext);
        }
        _values.push_back(str_value);
        reader.move();
    }    
    std::size_t children_status = this->children_process(reader);
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
std::size_t ValueCommand<T>::read_input(FileReader& reader) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    reader.move();

    std::pair<T, bool> conversion = this->convert_value(key, reader);
    if (!conversion.second) {
        std::string filecontext = reader.context_error();
        std::string str_value = reader.get_word();
        file_input_error(translate("ERROR_TYPE_CONVERSION", {key, str_value}), filecontext);
    }
    
    _value = conversion.first;
    reader.move();

    std::size_t children_status = this->children_process(reader);
    return 0;
}

template<typename T>
std::size_t VectorCommand<T>::read_input(FileReader& reader) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
    reader.move();

    std::string str_value = reader.get_word();
    // Read the values until the expected type conversion fails.
    while (str_value.size() > 0) {
        std::pair<T, bool> conversion = this->convert_value(key, reader); 
        if (!conversion.second) break;
        _values.push_back(conversion.first);
        
        reader.move();
        str_value = reader.get_word();
    }
    
    std::size_t children_status = this->children_process(reader);
    return 0;
}

template<typename T>
std::size_t MixCommand<T>::read_input(FileReader& reader) {
    std::string key = reader.get_word();
    if (!this->is_same_keyword(key)) return 1;
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
        reader.move();
    }
    
    std::size_t children_status = this->children_process(reader);
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