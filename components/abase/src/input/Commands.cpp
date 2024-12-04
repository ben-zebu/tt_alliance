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

template<typename T>
std::size_t ValueCommand<T>::read_input(FileReader& reader) {
    std::string key = reader.get_word();
    if (!is_same_keyword(key)) return 1;
    reader.move();

    std::string str_value = reader.get_word();

    // Convert the string to the expected type.
    std::stringstream ss(str_value);
    ss >> _value;
    std::cout << "str_value: " << str_value << "  _value: " << _value << std::endl;
    if (ss.fail() || !ss.eof()) {
        std::string filecontext = reader.context_error();
        file_input_error(translate("ERROR_TYPE_CONVERSION", {key, str_value}), filecontext);
    }

    reader.move();
    return 0;
}

template<typename T>
std::size_t VectorCommand<T>::read_input(FileReader& reader) {
    if (!is_same_keyword(reader.get_word())) return 1;
    reader.move();

    std::string str_value = reader.get_word();
    // Read the values until the expected type conversion fails.
    while (str_value.size() > 0) {
        T _value;
        // Convert the string to the expected type and add it to the vector.
        // Exit the loop if the conversion fails.
        std::stringstream ss(str_value);
        ss >> _value;
        if (ss.fail() || !ss.eof()) break;
        _values.push_back(_value);
        
        reader.move();
        str_value = reader.get_word();
    }
    
    std::size_t children_status = children_process(reader);
    return 0;
}

// Explicit template instantiation
template class ValueCommand<std::string>;
template class ValueCommand<int>;
template class ValueCommand<double>;
template class ValueCommand<std::size_t>;

template class VectorCommand<std::string>;
template class VectorCommand<int>;
template class VectorCommand<double>;
template class VectorCommand<std::size_t>;