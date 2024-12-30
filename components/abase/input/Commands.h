#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "FileReader.h"

namespace abase {

    /**
     * @brief Command system for input file processing
     *
     * This namespace contains a hierarchical command system for processing input files.
     * It provides base classes and specialized command types for different data handling needs.
     *
     * The command hierarchy consists of:
     * - BaseCommand: Abstract base class for all commands
     * - CompositeCommand: Intermediate class supporting child commands
     * - SingleCommand: Command for handling string values
     * - StringCommand: Command for handling single string values
     * - VectorStringCommand: Command for handling vectors of string values
     * - MixStringCommand: Command for handling a fixed number of string values
     * - NumericCommand<T>: Template command for handling numerical values
     * - ValueCommand<T>: Template command for handling single numerical values of type T
     * - VectorCommand<T>: Template command for handling vectors of numerical values of type T
     * - MixCommand<T>: Template command for handling a fixed number of numerical values of type T
     * - TimeStepCommand: Command for handling time steps
     *
     * Key features:
     * - Hierarchical command structure
     * - Support for command translations
     * - Type-safe value handling through templates
     * - Flexible input file processing
     *
     * Each command type serves a specific purpose:
     * - BaseCommand: Provides core functionality and interface
     * - CompositeCommand: Manages child commands and their processing
     * - SingleCommand: Handles string-based commands without values
     * - StringCommand: Processes single string values
     * - VectorStringCommand: Manages arrays/vectors of string values
     * - MixStringCommand: Handles a fixed number of string values
     * - NumericCommand<T>: Provides numerical value conversion support
     * - ValueCommand<T>: Processes single numerical values of any type
     * - VectorCommand<T>: Manages arrays/vectors of numerical values
     * - MixCommand<T>: Handles a fixed number of numerical values
     * - TimeStepCommand: Processes time steps
     *
     * Common usage:
     * @code
     * auto cmd = std::make_shared<SingleCommand>();
     * cmd->set_name("command_name");
     * cmd->read_input(reader);
     * @endcode
     *
     * @note All commands support nested hierarchies through the CompositeCommand base
     * @note Translation support is built into the base command level
     *
     * @see FileReader
     * @see BaseCommand
     * @see CompositeCommand
     */


    class CommandsCollector;

    /// @class BaseCommand
    /// @brief Base class for all commands
    class BaseCommand {
        protected:
            /// @brief command type
            std::string _type = "";
            /// @brief command name
            std::string _name = "";
            /// @brief command translations
            std::unordered_map<std::string, std::vector<std::string>> translations;
            /// @brief read status
            bool _read_status = false;

        public:
            //BaseCommand() = default;
            BaseCommand(const std::string& type) : _type(type) {};
            virtual ~BaseCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; }

            /// @brief Read the input file and set the values of the command
            /// @return Action status (0 for succes and 1 for failded)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) = 0;
            /// @brief  Get the type of the command
            /// @return the command type
            std::string get_type() const { return _type; }
            /// @brief Set the name of the command
            void set_name(const std::string& name) { _name = name; }
            /// @brief Returns the name of the command
            /// @return the command name
            std::string get_name() const { return _name; }
            /// @brief Set the translation of the command
            /// @param lang language of the translation
            /// @param translation translation of the command
            void set_translation(const std::string& lang, const std::vector<std::string>& translation) {
                translations[lang] = translation;
            }

            /// @brief Add a child command to the current one (pure virtual)
            /// @param child the sub command to add
            virtual void addChild(std::shared_ptr<BaseCommand> child) = 0;
            /// @brief Get the child command associated to a given name (pure virtual)
            /// @param name child command name
            /// @return pointer to the child command
            virtual std::shared_ptr<BaseCommand> get_child(const std::string& name) = 0;
            /// @brief Get the child command names (pure virtual)
            /// @return list of children's name
            virtual std::vector<std::string> get_children_names() const = 0;

            /// @brief Check if the command is the same as the given keyword
            /// @param a_key a word to compare with the command name
            /// @return status of the comparison
            bool is_same_keyword(const std::string& a_key);

            /// @brief Check if the input name is the command name or a child one (pure virtual)
            /// @param name name to check
            /// @return status of the comparison
            virtual bool is_command_name(const std::string& name) = 0;

            /// @brief Return a string value of the command (pure virtual)
            /// @param value returned value
            virtual void get_value(std::string& value) const { if (_read_status) value = ""; }
            /// @brief Return a size_t value of the command (pure virtual)
            /// @param value returned value
            virtual void get_value(std::size_t& value) const { if (_read_status) value = 0; }
            /// @brief Return a integer value of the command (pure virtual)
            /// @param value returned value
            virtual void get_value(int& value) const { if (_read_status) value = 0; }
            /// @brief Return a double value of the command (pure virtual)
            /// @param value returned value
            virtual void get_value(double& value) const {if (_read_status)  value = 0.; }

            /// @brief Return string values of the command (pure virtual)
            /// @param value returned values
            virtual void get_values(std::vector<std::string>& values) const { if (_read_status) values.clear(); }
            /// @brief Return size_t values of the command (pure virtual)
            /// @param value returned values
            virtual void get_values(std::vector<std::size_t>& values) const { if (_read_status) values.clear(); }
            /// @brief Return integer values of the command (pure virtual)
            /// @param value returned values
            virtual void get_values(std::vector<int>& values) const { if (_read_status) values.clear(); }
            /// @brief Return double values of the command (pure virtual)
            /// @param value returned values
            virtual void get_values(std::vector<double>& values) const { if (_read_status) values.clear(); }

    };

    /// @class CompositeCommand
    /// @brief Command class used to add children of the current command
    class CompositeCommand : public BaseCommand {
        protected:
            /// @brief List of subcommands associate to the current command
            std::vector<std::shared_ptr<BaseCommand>> _children;

            /// @brief Read input process for all children
            virtual std::size_t children_process(FileReader& reader, const CommandsCollector& collector);

            /// @brief clear read data if all children
            void children_clear();

        public:
            CompositeCommand(const std::string& type) : BaseCommand(type) {};
            virtual ~CompositeCommand() = default;
            /// @brief Add a child command to the current one
            /// @param child the sub command to add
            void addChild(std::shared_ptr<BaseCommand> child) override;

            /// @brief Get the child command associated to a given name
            /// @param name child command name
            /// @return pointer to the child command
            std::shared_ptr<BaseCommand> get_child(const std::string& name) override;

            /// @brief Check if the input name is the command name or a child one
            /// @param name name to check
            /// @return status of the comparison
            bool is_command_name(const std::string& name) override;
            /// @brief Return the list of children's name
            /// @return list of children's name
            std::vector<std::string> get_children_names() const;
    };

    namespace {
        /// @brief Get the value of a child command
        /// @tparam T type of the value 
        /// @param command main command
        /// @param child_name child command name
        /// @param value value associated to the child command
        template<typename T>
        void get_child_value(const std::shared_ptr<abase::BaseCommand>& command, const std::string& child_name, T& value) {
            auto sub = command->get_child(child_name);
            if (sub) sub->get_value(value);
        }

        /// @brief Get a vector of values from a child command
        /// @tparam T type of the values
        /// @param command main command
        /// @param child_name child command name
        /// @param values vector of values associated to the child command
        template<typename T>
        void get_child_values(const std::shared_ptr<abase::BaseCommand>& command, const std::string& child_name, std::vector<T>& values) {
            auto sub = command->get_child(child_name);
            if (sub) sub->get_values(values);
        }
    }

}

