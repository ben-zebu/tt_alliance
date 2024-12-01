#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace abase {
    class BaseCommand;

    /// @brief Factory class to create all commands by predefined types
    class CommandsTypeFactory {
        private:
            /// @brief Collection of all commands available by their type
            static std::unordered_map<std::string, std::shared_ptr<BaseCommand>> registered_commands;

        public:
            /// @brief Create a command object by its type
            /// @param name type of the command
            /// @return the command object
            static std::shared_ptr<BaseCommand> get_object(const std::string& name);
            /// @brief Set the creator of the command with its type
            /// @param name type of the command
            /// @param creator creator of the command
            static bool set_creator(const std::string& name, std::shared_ptr<BaseCommand> creator);
            /// @brief Clear the factory
            static void clear() { registered_commands.clear(); }    
    };

}