#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "FileReader.h"

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
            void addChild(std::shared_ptr<BaseCommand> child) {
                _children.push_back(std::move(child));
            }

            /// @brief Get the child command associated to a given name
            /// @param name child command name
            /// @return pointer to the child command
            std::shared_ptr<BaseCommand> get_child(const std::string& name);

    };

    class SingleCommand : public CompositeCommand {
        protected :
            /// @brief Value of the command read from the input file
            std::string _value;
            
        public :
            SingleCommand() = default;
            virtual ~SingleCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader);
            /// @brief Return the string associated to the command
            /// @return the value
            std::string get() { return _value; }
    };

    template<typename T>
    class ValueCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            T _value;
            
        public :
            ValueCommand() = default;
            virtual ~ValueCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader);
            /// @brief Return the value associated to the command
            /// @return the value
            T get() { return _value; }

    };

    template<typename T>
    class VectorCommand : public CompositeCommand {
        protected :
            /// @brief Vector of values associated to the command
            std::vector<T> _values;
            
        public :
            VectorCommand() = default;
            virtual ~VectorCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader);
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @param size expected size of the vector
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const std::size_t& size);
            /// @brief Return the values associated to the command
            /// @return the vector of values
            std::vector<T> get() { return _values; }
    };


}

