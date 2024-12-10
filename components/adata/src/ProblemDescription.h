#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata {

    class ProblemDescription {
        public:
            std::string code_type = "";
            std::string code_edition = "";

            std::size_t category = 0;
            std::size_t earthquakes = 0;
            
            std::string language = "";
            std::string units = "mpa";
            std::size_t edition = 0;

            std::vector<std::string> titles;

            ProblemDescription() = default;
            virtual ~ProblemDescription() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            void init(const std::shared_ptr<abase::BaseCommand>& command);

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            void verify(const std::string& filecontext) const;
    };

} // namespace adata