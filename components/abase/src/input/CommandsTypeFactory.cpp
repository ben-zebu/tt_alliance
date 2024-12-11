#include "Commands.h"
#include "CommandsNumerics.h"
#include "CommandsSpecial.h"
#include "CommandsString.h"

#include "Environment.h"
#include "CommandsTypeFactory.h"

using namespace abase;

// Define the static member
std::unordered_map<std::string, std::function<std::shared_ptr<BaseCommand>()>> CommandsTypeFactory::registered_creators;

std::shared_ptr<BaseCommand> CommandsTypeFactory::create_command(const std::string& name) {
    auto it = registered_creators.find(name);
    if (it == registered_creators.end()) {
        error(translate("ERROR_FACTORY_UNKNOWN", name));
        return nullptr;
    }
    return it->second();
}

bool CommandsTypeFactory::register_creator(const std::string& name, std::function<std::shared_ptr<BaseCommand>()> creator) {
    if (registered_creators.find(name) != registered_creators.end()) {
        error(translate("ERROR_FACTORY_MULTI_CREATION", name));
        return false;
    }
    
    registered_creators[name] = std::move(creator);
    return true;
}

// Initialize the registry with all command types
namespace {
    struct RegisterCommands {
        RegisterCommands() {
            CommandsTypeFactory::register_creator("single", []() { return std::make_shared<SingleCommand>(); });
            CommandsTypeFactory::register_creator("string", []() { return std::make_shared<StringCommand>(); });
            CommandsTypeFactory::register_creator("int", []() { return std::make_shared<ValueCommand<int>>(); });
            CommandsTypeFactory::register_creator("real", []() { return std::make_shared<ValueCommand<double>>(); });
            CommandsTypeFactory::register_creator("uint", []() { return std::make_shared<ValueCommand<std::size_t>>(); });
            CommandsTypeFactory::register_creator("string_array", []() { return std::make_shared<VectorStringCommand>(); });
            CommandsTypeFactory::register_creator("int_array", []() { return std::make_shared<VectorCommand<int>>(); });
            CommandsTypeFactory::register_creator("real_array", []() { return std::make_shared<VectorCommand<double>>(); });
            CommandsTypeFactory::register_creator("uint_array", []() { return std::make_shared<VectorCommand<std::size_t>>(); });
            CommandsTypeFactory::register_creator("string_mix", []() { return std::make_shared<MixStringCommand>(); });
            CommandsTypeFactory::register_creator("int_mix", []() { return std::make_shared<MixCommand<int>>(); });
            CommandsTypeFactory::register_creator("real_mix", []() { return std::make_shared<MixCommand<double>>(); });
            CommandsTypeFactory::register_creator("uint_mix", []() { return std::make_shared<MixCommand<std::size_t>>(); });
            CommandsTypeFactory::register_creator("time_array", []() { return std::make_shared<TimeStepCommand>(); });
            CommandsTypeFactory::register_creator("table_array", []() { return std::make_shared<TableCommand>(); });
            CommandsTypeFactory::register_creator("file", []() { return std::make_shared<FileCommand>(); });
        }
    } registerCommands;
}
