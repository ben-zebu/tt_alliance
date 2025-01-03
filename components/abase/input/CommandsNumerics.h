#pragma once

#include "Commands.h"

namespace abase {

    /// @class NumericCommand
    /// @brief Virtual class used to support numerical values conversion for its child classes
    template<typename T>
    class NumericCommand : public CompositeCommand {
        protected:
            std::pair<T, bool> convert_value(const std::string& key, FileReader& reader);

        public:
            NumericCommand(const std::string& type) : CompositeCommand(type) {};
            virtual ~NumericCommand() = default;

    };

    /// @class ValueCommand
    /// @brief Class used to read a numerical value from the input file
    template<typename T>
    class ValueCommand : public NumericCommand<T> {
        protected :
            /// @brief Value associated to the command
            T _value;

        public :
            ValueCommand(const std::string& type) : NumericCommand<T>(type) {};
            virtual ~ValueCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { this->_read_status = false; _value = T(); this->children_clear(); }

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the value of the command
            /// @param value value associated to the command
            void get_value(T& value) const override { if (this->_read_status) value = _value; }

    };

    /// @class VectorCommand
    /// @brief Class used to read a list of numerical values from the input file
    template<typename T>
    class VectorCommand : public NumericCommand<T> {
        protected :
            /// @brief Vector of values associated to the command
            std::vector<T> _values;

        public :
            VectorCommand(const std::string& type) : NumericCommand<T>(type) {};
            virtual ~VectorCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { this->_read_status = false; _values.clear(); this->children_clear(); }

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the values of the command
            /// @param values values associated to the command
            void get_values(std::vector<T>& values) const override { if (this->_read_status) values = _values; }
    };

    /// @class MixCommand
    /// @brief Class used to read a fixed number of numerical values from the input file
    template<typename T>
    class MixCommand : public NumericCommand<T> {
        protected :
            /// @brief Vector of values associated to the command
            std::vector<T> _values;
            /// @brief Number of values to read
            std::size_t _n_values;

        public :
            MixCommand(const std::string& type) : NumericCommand<T>(type) {};
            virtual ~MixCommand() = default;

            /// @brief clear read data associated to the command
            virtual void clear() { this->_read_status = false; _values.clear(); _n_values = 0; this->children_clear(); }

            /// @brief Read input file and set the value of the command
            /// @param reader file reader associated to the input file
            /// @return a status code (0 for success and 1 for fail)
            virtual std::size_t read_input(FileReader& reader, const CommandsCollector& collector) override;
            /// @brief Get the values of the command
            /// @param values values associated to the command
            void get_values(std::vector<T>& values) const override { if (this->_read_status) values = _values; }
    };
}

