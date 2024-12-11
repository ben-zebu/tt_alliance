#pragma once

#include "Commands.h"

namespace abase {

    /// @class StringCommand
    /// @brief Command used to read a string value from the input file
    class StringCommand : public CompositeCommand {
        protected :
            /// @brief Value associated to the command
            std::string _value;
            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _value = ""; }

        public :
            StringCommand() = default;
            virtual ~StringCommand() = default;
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
            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _values.clear(); }

        public :
            VectorStringCommand() = default;
            virtual ~VectorStringCommand() = default;
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
            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _values.clear(); _n_values = 0; }

        public :
            MixStringCommand() = default;
            virtual ~MixStringCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the values of the command
            /// @param values values associated to the command
            void get_values(std::vector<std::string>& values) const override { if (_read_status) values = _values; }

    };

}

