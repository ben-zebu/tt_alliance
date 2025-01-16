#pragma once

#include "Table.h"
#include "Commands.h"
#include "Environment.h"
#include "FileReader.h"

namespace adata::parts {

    /// @class ProblemTable
    /// @brief Table of values read from the input file
    class ProblemTable {
        private:
            /// @brief Default name for the table
            void default_name(std::size_t id) { name = "TABLE_" + std::to_string(id); };
            
        public:
            /// @brief Name of the table
            std::string name = "";
            /// @brief Table of values
            amath::Table table;

            ProblemTable() = default;
            virtual ~ProblemTable() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            /// @param id rank associated to the table
            void init(const std::shared_ptr<abase::BaseCommand>& command, std::size_t id);
            /// @brief Verify the coherence of object values
            /// @param filecontext file content
            void verify(const std::string& filecontext) const;
    };
}