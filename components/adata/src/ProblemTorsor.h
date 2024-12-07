#pragma once

#include "Environment.h"
#include "Commands.h"
#include "FileReader.h"

namespace adata {

    class ProblemTorsor {
        public:
            /// @brief Rank of each card that contains stress values associated to external torsors
            std::vector<std::size_t> cards;

            ProblemTorsor() = default;
            virtual ~ProblemTorsor() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            /// @param reader object used to read the input file
            void init(std::shared_ptr<abase::BaseCommand> command);

            /// @brief Verify if the object is correctly initialized
            /// @param filecontext context of the input file
            /// @return status of the object initialization
            void verify(std::string& filecontext) const;
    };

}