#pragma once

#include "Commands.h"
#include "Environment.h"
#include "FileReader.h"

namespace adata::parts {
    
    /// @class ProblemFemInterface
    /// @brief Interface to the finite element results file 
    class ProblemFemInterface {
        private:
            /// @brief build fem results file name and check if the file exists
            /// @param sequence list of file names
            void set_fem_results_file(const std::string& sequence);

        public:
            /// @brief name of the FEM file
            std::string filename = "";

            ProblemFemInterface() = default;
            virtual ~ProblemFemInterface() = default;

            /// @brief Initialize the object with the values read from the input file
            /// @param command values read from the input file
            void init(const std::shared_ptr<abase::BaseCommand>& command);
            /// @brief Verify the coherence of object values
            /// @param filecontext file content
            void verify(const std::string& filecontext) const;
    };

}