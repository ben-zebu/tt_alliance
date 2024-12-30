#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace abase {
    class BaseCommand;

    /// @brief Factory class to create all commands by predefined types
    class CommandsTypeFactory {
        private:
            /// @brief Collection of all command creators available by their type
            static std::unordered_map<std::string, std::function<std::shared_ptr<BaseCommand>()>> registered_creators;

        public:
            /// @brief Create a command object by its type
            /// @param name type of the command
            /// @return the command object
            static std::shared_ptr<BaseCommand> create_command(const std::string& name);
            /// @brief Set the creator of the command with its type
            /// @param name type of the command
            /// @param creator function to create the command
            static bool register_creator(const std::string& name, std::function<std::shared_ptr<BaseCommand>()> creator);
            /// @brief Clear the factory
            static void clear() { registered_creators.clear(); }    
    };

}
