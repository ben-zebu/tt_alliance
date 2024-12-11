#include "Environment.h"

#include "ProblemDescription.h"
#include "ProblemFemInterface.h"
#include "ProblemLoadstep.h"
// #include "ProblemMaterial.h"
#include "ProblemPlate.h"
// #include "ProblemSection.h"
#include "ProblemTable.h"
#include "ProblemTorsor.h"
#include "ProblemTransient.h"

namespace adata {

    class DataManager {
        private:
            /// @brief Convert read data from input file into data objects
            /// @param command read data from the input file
            /// @param filecontext file context
            void set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext);

            /// @brief Read the title of the problem in the input file
            /// @param reader file reader associated to the input file
            /// @param collector commands collector used to read the input file
            /// @return problem title as a vector of strings
            std::vector<std::string> read_title(abase::FileReader& reader, const abase::CommandsCollector& collector);

        protected:
            ProblemFemInterface fem_interface;
            ProblemDescription description;
            ProblemPlate plate;
            ProblemTorsor torsor;
            std::vector<ProblemLoadstep> loadsteps;
            std::vector<ProblemTransient> transients;
            std::vector<ProblemTable> tables;
            // std::vector<ProblemMaterial> materials;
            // std::vector<ProblemSection> sections;

        public:
            DataManager() = default;
            ~DataManager() = default;
            /// @brief Function used to read the input file based on a commands collector
            /// @param filename name of the input file
            /// @param commands_tree_file file containing the commands tree definition
            void read_data(const std::string& filename, const std::string& commands_tree_file);
            /// @brief Verify if the object is correctly initialized with consistent objects
            void verify() const;
    };

}