#pragma once

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

namespace adata {
    /// @brief Class used to read the command line and to set parameter in the global configuration manager
    class CommandParser {
        private:
            /// @brief show help message
            void help();
            /// @brief show the program version
            void version();
            /// @brief Set an integer parameter in the global configuration manager
            /// @param key parameter name
            /// @param value parameter value
            void set_integer_option(const std::string& key, const std::string& value) const;
            /// @brief Set a boolean parameter (internaly set an inetger to 1) in the global configuration manager
            /// @param key parameter name
            void set_boolean_option(const std::string& key) const;
            /// @brief Set a parameter in the global configuration manager for the application path
            /// @param path path given by the command line
            void set_application_path(const std::string& path);
        
        protected:
            std::vector<std::string> integer_options = { 
                "--multi", "--mpi", "--optimize", "--verbose", "--max_load_set_cat2"
            };
            std::vector<std::string> boolean_options = { 
                "--no_omp_nested", "--only_min_max_torseur", "--compatibility", "--plate_compatibility" 
            };

        public:
            /// @brief  Parse the command line
            /// @param argc Number of arguments on the command line
            /// @param argv arguments of the command line
            CommandParser(int argc, char** argv);
    };

}