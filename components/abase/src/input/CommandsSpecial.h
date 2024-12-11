#pragma once

#include "Commands.h"

namespace abase {

    /// @class SingleCommand
    /// @brief Command without value
    class SingleCommand : public CompositeCommand {
        protected :
            /// @brief keyword read from the input fila associated to the command
            std::string _value;
            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _value = ""; }
        public :
            SingleCommand() = default;
            virtual ~SingleCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reade, const CommandsCollector& collector) override;
            /// @brief Get the value of the command
            /// @param value value associated to the command
            void get_value(std::string& value) const override { if (_read_status) value = _value; }
    };

    /// @class TimeStepCommand
    /// @brief Class used to read time steps from the input file
    class TimeStepCommand : public CompositeCommand {
        private:
            /// @brief Convert a time step range into a vector of time steps
            /// @param start first time step
            /// @param end last time step
            /// @param step time step increment
            /// @param status status of the conversion
            /// @return a vector of time steps
            std::vector<std::size_t> convert_range(const std::string& start, const std::string& end,
                                                   const std::string& step, bool& status);
            /// @brief Extract all time steps from a sequence of strings
            /// @param sequence extracted sequence
            /// @param status status of the extraction
            /// @return a vector of time steps
            std::vector<std::size_t> split_sequence(const std::string& sequence, bool& status);
        protected :
            /// @brief vector of time steps rank.
            std::vector<std::size_t> _values;
            /// @brief clear read data associated to the command
            virtual void clear() { this->_read_status = false; _values.clear(); }

        public:
            TimeStepCommand() = default;
            virtual ~TimeStepCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the values of the command
            /// @param values values associated to the command
            void get_values(std::vector<std::size_t>& values) const override { if (_read_status) values = _values; }
    };

    /// @class TableCommand
    /// @brief Class used to read a table from the input file
    class TableCommand : public CompositeCommand {
        private:
            /// @brief Convert table values from a sequence of strings
            /// @param values sequence of strings
            /// @param status status of the conversion
            void convert(const std::vector<std::string>& values, bool& status);
            /// @brief Extract all time steps from a sequence of strings
            /// @param sequence extracted sequence
            /// @param status status of the extraction
            void split_sequence(const std::string& sequence, bool& status);
        protected :
            /// @brief table name
            std::string _table_name;
            /// @brief vector of time steps rank.
            std::vector<double> _values;
            /// @brief clear read data associated to the command
            virtual void clear() { this->_read_status = false; _values.clear(); }

        public:
            TableCommand() = default;
            virtual ~TableCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the table name
            /// @param name name of the table
            void get_value(std::string& name) const override { if (_read_status) name = _table_name; }
            /// @brief Get the values of the command
            /// @param values values associated to the command
            void get_values(std::vector<double>& values) const override { if (_read_status) values = _values; }
            
    };

    /// @class FileCommand
    /// @brief Class used to read the name of a file from the input file 
    class FileCommand : public CompositeCommand {
        protected :
            /// @brief file name
            std::string _value;
            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _value = ""; }

        public:
            FileCommand() = default;
            virtual ~FileCommand() = default;
            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the table name
            /// @param name name of the table
            void get_value(std::string& name) const override { if (_read_status) name = _value; }            
    };

}

