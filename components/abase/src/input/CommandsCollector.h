#pragma once

#include "Commands.h"
#include "CommandsTypeFactory.h"
#include "Environment.h"

namespace abase {

    /// @brief Factory class to create all commands read from an external file. This factory is defined by a YAML file. 
    /// It is different from the CommandsTypeFactory class which is used to store all commands by their type.
    
    /// The factory is a singleton
    class CommandsCollector {
        private:
            /// @brief Collection of all commands available: name and associated command
            std::unordered_map<std::string, std::shared_ptr<BaseCommand>> commands;

        public:
            CommandsCollector() = default;
            ~CommandsCollector() = default;

            /// @brief Load all commands from a YAML file
            /// @param filename path to the YAML file
            void loadCommandsFromFile(const std::string& filename);

            /// @brief Get the command associated to a given name
            /// @param name command name
            /// @return the command object
            std::shared_ptr<BaseCommand> get_command(const std::string& name);
            /// @brief Get the list of all commands names
            /// @return the list of commands names
            std::vector<std::string> get_commands_names();


            void clear() { commands.clear(); }
    };

}