#pragma once

#include "Table.h"
#include "Commands.h"
#include "Environment.h"
#include "FileReader.h"

namespace adata {

    class ProblemTable {
        private:
            void default_name(std::size_t id) { name = "TABLE_" + std::to_string(id); };
            
        public:
            std::string name = "";
            amath::Table table;

            ProblemTable() = default;
            virtual ~ProblemTable() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            /// @param id rank associated to the table
            void init(std::shared_ptr<abase::BaseCommand> command, std::size_t id);

            void verify(std::string& filecontext) const;
    };
}