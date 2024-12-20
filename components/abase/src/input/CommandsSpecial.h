#pragma once

#include "Commands.h"

namespace abase {

    /// @class SingleCommand
    /// @brief Command without value
    class SingleCommand : public CompositeCommand {
        protected :
            /// @brief keyword read from the input fila associated to the command
            std::string _value;

        public :
            SingleCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~SingleCommand() = default;

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

        public:
            TimeStepCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~TimeStepCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { this->_read_status = false; _values.clear(); children_clear(); } 

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

        public:
            TableCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~TableCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { this->_read_status = false; _values.clear(); children_clear(); } 

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

    /// @class CoefficientCommand
    /// @brief Class used to read a coefficient from the input file
    class CoefficientCommand : public CompositeCommand {
        private:
            /// @brief Convert coefficient values from a sequence of strings
            /// @param values sequence of strings
            /// @param status status of the conversion
            void convert(const std::vector<std::string>& values, bool& status);
            /// @brief Extract all time steps from a sequence of strings
            /// @param sequence extracted sequence
            /// @param key key associated to the coefficient
            /// @param filecontext file context for error message
            void split_sequence(const std::string& sequence, const std::string & key, const std::string& filecontext);
        protected :
            /// @brief table name associated to the coefficient
            std::string _table_name;
            /// @brief Values associated to the coefficient (expected size is 0 or 1)
            std::vector<double> _values;

        public:
            CoefficientCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~CoefficientCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _table_name = ""; _values.clear(); children_clear(); }

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the table name associated to the coefficient
            /// @param name name of the table
            void get_value(std::string& name) const override { if (_read_status) name = _table_name; }
            /// @brief Get the values of the command associated to the coefficient
            /// @param values values associated to the command
            void get_values(std::vector<double>& values) const override { if (_read_status) values = _values; }
    };

    /// @class FileCommand
    /// @brief Class used to read the name of a file from the input file 
    class FileCommand : public CompositeCommand {
        protected :
            /// @brief file name
            std::string _value;

        public:
            FileCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~FileCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { _read_status = false; _value = ""; children_clear(); }

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the table name
            /// @param name name of the table
            void get_value(std::string& name) const override { if (_read_status) name = _value; }            
    };



}

