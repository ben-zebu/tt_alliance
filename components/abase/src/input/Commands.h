#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "FileReader.h"

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
 * - ValueCommand<T>: Template command for handling single values of type T
 * - VectorCommand<T>: Template command for handling vectors of values of type T
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
 * - SingleCommand: Handles string-based commands
 * - ValueCommand<T>: Processes single values of any type
 * - VectorCommand<T>: Manages arrays/vectors of values
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
namespace abase {

    /// @brief Base class for all commands
    class BaseCommand {
        protected:
            std::string _name = "";    
            std::unordered_map<std::string, std::vector<std::string>> translations;

        public:
            BaseCommand() = default;
            BaseCommand(const std::string& name) : _name(name) {}
            virtual ~BaseCommand() = default;
            /// @brief Read the input file and set the values of the command
            /// @return Action status (0 for succes and 1 for failded)
            virtual std::size_t read_input(FileReader& reader) = 0;
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

            /// @brief Check if the command is the same as the given keyword
            /// @param a_key a word to compare with the command name
            /// @return status of the comparison
            bool is_same_keyword(const std::string& a_key);

            /// @brief Check if the input name is the command name or a child one (pure virtual)
            /// @param name name to check
            /// @return status of the comparison
            virtual bool is_command_name(const std::string& name) = 0;
    };

    /// @brief Command class used to add children of the current command
    class CompositeCommand : public BaseCommand {
        protected:
            /// @brief List of subcommands associate to the current command
            std::vector<std::shared_ptr<BaseCommand>> _children;

            /// @brief Read input process for all children
            virtual std::size_t children_process(FileReader& reader);

        public:
            CompositeCommand() = default;
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
            bool is_command_name(const std::string& name);

    };

    class SingleCommand : public CompositeCommand {
        public :
            SingleCommand() = default;
            virtual ~SingleCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader) override;
    };

    //
    // Classes for string values
    //

    class StringCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            std::string _value;

        public :
            StringCommand() = default;
            virtual ~StringCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader) override;
    };

    class VectorStringCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            std::vector<std::string> _values;

        public :
            VectorStringCommand() = default;
            virtual ~VectorStringCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader) override;
    };

    class MixStringCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            std::vector<std::string> _values;
            /// @brief Number of values to read
            std::size_t _n_values;

        public :
            MixStringCommand() = default;
            virtual ~MixStringCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader) override;
    };

    //
    // Template class for numerical values
    //

    template<typename T>
    class NumericCommand : public CompositeCommand {
        protected:
            std::pair<T, bool> convert_value(const std::string& key, FileReader& reader);

        public:
            NumericCommand() = default;
            virtual ~NumericCommand() = default;

    };

    template<typename T>
    class ValueCommand : public NumericCommand<T> {
        protected :
            /// @brief Value associated to the command
            T _value;
            
        public :
            ValueCommand() = default;
            virtual ~ValueCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader) override;
    };

    template<typename T>
    class VectorCommand : public NumericCommand<T> {
        protected :
            /// @brief Vector of values associated to the command
            std::vector<T> _values;
            
        public :
            VectorCommand() = default;
            virtual ~VectorCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader) override;
    };

    template<typename T>
    class MixCommand : public NumericCommand<T> {
        protected :
            /// @brief Vector of values associated to the command
            std::vector<T> _values;
            /// @brief Number of values to read
            std::size_t _n_values;
            
        public :
            MixCommand() = default;
            virtual ~MixCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader) override;
    };

}

