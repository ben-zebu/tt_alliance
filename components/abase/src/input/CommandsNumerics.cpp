#include <algorithm>
#include <cctype>
#include <sstream>
#include <cassert>

#include "CommandsNumerics.h"
#include "CommandsCollector.h"
#include "Environment.h"

using namespace abase;

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
