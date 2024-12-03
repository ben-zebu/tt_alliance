#include <algorithm>
#include <cctype>
#include <sstream>
#include <cassert>

#include "Commands.h"
#include "Environment.h"

using namespace abase;

std::string to_lowercase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}

bool BaseCommand::is_same_keyword(const std::string& a_key) {
    std::string lower_key = to_lowercase(a_key);
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
    if (!is_same_keyword(reader.get_word())) return 1;
    reader.move();

    std::string str_value = reader.get_word();
    try {
        std::stringstream ss(str_value);
        ss >> _value;
    }
    catch (std::invalid_argument& e) {
        input_error(translate("ERROR_READ_VALUE", {str_value}));
    }
    reader.move();
    return 0;
}

template<typename T>
std::size_t VectorCommand<T>::read_input(FileReader& reader) {
    if (!is_same_keyword(reader.get_word())) return 1;
    reader.move();

    std::string str_value = reader.get_word();
    while (str_value.size() > 0) {
        T _value;
        std::stringstream ss(str_value);
        try {
            ss >> _value;
            _values.push_back(_value);
        }
        catch(const std::exception& e) {
            break;
        }
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