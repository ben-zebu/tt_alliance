#pragma once
#include "Environment.h"
#include "CommandsCollector.h"
#include "FileReader.h"

namespace abase {

    class DataCollector {
        protected:
            /// @brief Convert read data from input file into data objects
            /// @param command read data from the input file
            /// @param filecontext file context
            virtual void set_data(const std::shared_ptr<BaseCommand>& command, const std::string& filecontext) = 0;

            /// @brief Read special line without any command
            /// @param command_name name of the command linked to the special line
            /// @param reader file reader associated to the input file
            /// @param collector commands collector used to read the input file
            /// @note By default, empty implementation
            virtual void read_special_line(const std::string& command_name, FileReader& reader, 
                                           const CommandsCollector& collector) {};

        public:
            DataCollector() = default;
            virtual ~DataCollector() = default;
            /// @brief Function used to read the input file based on a commands collector
            /// @param filename name of the input file
            /// @param commands_tree_file file containing the commands tree definition
            virtual void read_data(const std::string& filename, const std::string& commands_tree_file);
            /// @brief Verify if the object is correctly initialized with consistent objects
            /// @note By default, empty implementation
            virtual void verify() const {};
    };

}