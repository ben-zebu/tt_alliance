#include "Environment.h"

#include "ProblemDescription.h"
#include "ProblemTorsor.h"
#include "ProblemLoadstep.h"

namespace adata {

    class DataManager {
        private:
            void set_data(std::shared_ptr<abase::BaseCommand> command, std::string filecontext);


            /// @brief Read the title of the problem in the input file
            /// @param reader file reader associated to the input file
            /// @param collector commands collector used to read the input file
            /// @return problem title as a vector of strings
            std::vector<std::string> read_title(abase::FileReader& reader, const abase::CommandsCollector& collector);

        protected:
            ProblemDescription description;
            ProblemTorsor torsor;
            std::vector<ProblemLoadstep> loadsteps;
        /*
        protected:
            pb_fem_interface fem_interface;
            pb_description description;
            pb_plate plate_description;
            pb_torsor torsor;
            std::vector<pb_loadstep> loadsteps;
            std::vector<pb_transient> transients;
            std::vector<pb_table> tables;
            std::vector<pb_material> materials;
            std::vector<pb_section> sections;
        */
        public:
            DataManager() = default;
            ~DataManager() = default;
            /// @brief Function used to read the input file based on a commands collector
            /// @param filename name of the input file
            /// @param commands_tree_file file containing the commands tree definition
            void read_data(const std::string& filename, const std::string& commands_tree_file);
    };

}