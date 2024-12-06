#include "Commands.h"
#include "CommandsCollector.h"
#include "CommandsTypeFactory.h"
#include "Environment.h"
#include "Filesystem.h"

#include "yaml-cpp/yaml.h"

using namespace abase;

/// @brief Get the command type from the YAML node position (internal function without outside access)
/// @param node position in the YAML tree
/// @return the command type
std::string get_command_type(const YAML::Node& node) {
    for (const auto& key : node) {
        if (key.first.as<std::string>() == "type") return key.second.as<std::string>();
    }
    return "";
}

/// @brief Get the command keys from the YAML node position (internal function without outside access)
/// @param node position in the YAML tree
/// @return the associated keys
std::unordered_map<std::string, std::vector<std::string>> get_command_keys(const std::string& node_name, 
                                                                           const YAML::Node& node) {
    std::unordered_map<std::string, std::vector<std::string>> translations;

    for (const auto& key : node) {
        std::string str_key = key.first.as<std::string>();
        if (str_key != "keys") continue;
        for (const auto& translation : key.second) {
            std::string lang = translation.first.as<std::string>();
            std::vector<std::string> keys = translation.second.as<std::vector<std::string>>();
            translations[lang] = keys;
        }
    }

    // if no keys are found, return an error
    if (translations.empty()) {
        error(translate("ERROR_FACTORY_UNDEFINED_PARAMETER", {"keys", node_name}));
    }

    return translations;
}

/// @brief Create a command object based on informattions read in YAML file (internal function without outside access).
/// The function is recursive to add all children commands.
/// @param node position in the YAML tree
/// @return the command object
std::shared_ptr<BaseCommand> add_command(const std::string node_name, const YAML::Node& node) {
    // get the command type
    std::string ntype = get_command_type(node);
    if (ntype.empty()) {
        error(translate("ERROR_FACTORY_UNDEFINED_PARAMETER", {"type", node_name}));
    }

    // create the command by its type
    std::shared_ptr<BaseCommand> command = CommandsTypeFactory::create_command(ntype);
    if (command == nullptr) {
        error(translate("ERROR_FACTORY_UNKNOWN", node_name));
    }

    // set the name and the translations
    command->set_name(node_name);
    for (const auto& translation : get_command_keys(node_name, node)) {
        command->set_translation(translation.first, translation.second);
    }
    
    //add children
    for (const auto& child : node) {
        std::string childname = child.first.as<std::string>();
        if (childname == "keys" || childname == "type") continue;
        command->addChild( add_command(childname, child.second) );
    }

    return command;
}

void CommandsCollector::loadCommandsFromFile(const std::string& filename) {
    // Check the file type and status before loading
    if ( !is_file_readable(filename) ) {
        error(translate("ERROR_FACTORY_FILE", filename));
        throw std::invalid_argument("Cannot read the file " + filename + " !");
    }

    // Clear previous commands
    clear();

    // Real the YAML file
    YAML::Node node = YAML::LoadFile(filename);

    // Add all commands and their children to the collector
    for (const auto& key : node) {
        std::string str_key = key.first.as<std::string>();
        commands[str_key] = add_command(str_key, key.second);
    }
}

std::shared_ptr<BaseCommand> CommandsCollector::get_command(const std::string& name) {
    if (commands.find(name) == commands.end()) {
        error(translate("ERROR_FACTORY_UNKNOWN", name));
    }
    return commands[name];
}

std::vector<std::string> CommandsCollector::get_commands_names() {
    std::vector<std::string> names;
    for (const auto& command : commands) {
        names.push_back(command.first);
    }
    return names;
}

bool CommandsCollector::is_command_name(const std::string& name) const {
    for (const auto& command : commands) {
        if (command.second->is_command_name(name)) return true;
    }
    return false;
}