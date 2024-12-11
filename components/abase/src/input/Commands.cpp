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
