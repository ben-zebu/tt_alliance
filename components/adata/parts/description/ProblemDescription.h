#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata::parts {

    /// @class ProblemDescription
    /// @brief Description of the problem read from the input file and stored general information
    class ProblemDescription {
        public:
            /// @brief Type of the problem : RCCM or ASME
            std::string code_type = "";
            /// @brief Code edition used for the problem
            std::string code_edition = "";

            /// @brief Category of the problem : 1, 2, 3, 4, 5 (as hydraulic test)
            std::size_t category = 0;
            /// @brief Number of earthquakes fluctuations between 2 loadsteps (\f$ N_s \f$)
            std::size_t earthquakes = 0;
            
            /// @brief Language used for output messages and files
            std::string language = "";
            /// @brief Stress units from finite element results
            std::string units = "mpa";
            /// @brief Level of edition for output files
            std::size_t edition = 0;
            /// @brief Title of the problem (can be multiple lines)
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

}