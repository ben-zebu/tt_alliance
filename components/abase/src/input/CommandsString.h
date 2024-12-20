#pragma once

#include "Commands.h"

namespace abase {

    /// @class StringCommand
    /// @brief Command used to read a string value from the input file
    class StringCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            std::string _value;

        public :
            StringCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~StringCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _value = ""; children_clear(); }

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the value of the command
            /// @param value value associated to the command
            void get_value(std::string& value) const override { if (_read_status) value = _value; }

    };

    /// @class VectorStringCommand
    /// @brief Command used to read a list of string values from the input file
    class VectorStringCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            std::vector<std::string> _values;

        public :
            VectorStringCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~VectorStringCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _values.clear(); children_clear(); }   

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the values of the command
            /// @param values values associated to the command
            void get_values(std::vector<std::string>& values) const override { if (_read_status) values = _values; }

    };

    /// @class MixStringCommand
    /// @brief Command used to read a fixed number of string values from the input file
    class MixStringCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            std::vector<std::string> _values;
            /// @brief Number of values to read
            std::size_t _n_values;

        public :
            MixStringCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~MixStringCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _values.clear(); _n_values = 0; children_clear(); }

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the values of the command
            /// @param values values associated to the command
            void get_values(std::vector<std::string>& values) const override { if (_read_status) values = _values; }

    };

}

