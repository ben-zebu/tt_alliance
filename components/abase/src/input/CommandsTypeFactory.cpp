#include "Commands.h"
#include "Environment.h"
#include "CommandsTypeFactory.h"

using namespace abase;

// Define the static member
std::unordered_map<std::string, std::shared_ptr<BaseCommand>> CommandsTypeFactory::registered_commands;

std::shared_ptr<BaseCommand> CommandsTypeFactory::get_object(const std::string& name) {
    if (registered_commands.find(name) == registered_commands.end()) {
        error(translate("ERROR_FACTORY_UNKNOWN", name));
    }
    return registered_commands[name];
}

bool CommandsTypeFactory::set_creator(const std::string& name, std::shared_ptr<BaseCommand> creator) {
    if (registered_commands.find(name) != registered_commands.end()) {
        error(translate("ERROR_FACTORY_MULTI_CREATION", name));
    }
    
    registered_commands[name] = creator;
    return true;
}

// Initialize the registry with all command types
namespace {
    struct RegisterCommands {
        RegisterCommands() {
            std::unordered_map<std::string, std::shared_ptr<BaseCommand>> commands = {
                {"single", std::make_shared<SingleCommand>()},
                {"string", std::make_shared<ValueCommand<std::string>>()},
                {"int", std::make_shared<ValueCommand<int>>()},
                {"real", std::make_shared<ValueCommand<double>>()},
                {"uint", std::make_shared<ValueCommand<std::size_t>>()},
                {"string_array", std::make_shared<VectorCommand<std::string>>()},
                {"int_array", std::make_shared<VectorCommand<int>>()},
                {"real_array", std::make_shared<VectorCommand<double>>()},
                {"uint_array", std::make_shared<VectorCommand<std::size_t>>()}
            };

            for (const auto& command : commands) {
                CommandsTypeFactory::set_creator(command.first, command.second);
            }
        }
    } registerCommands;
}
