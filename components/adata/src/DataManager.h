#include "Environment.h"

namespace adata {

    class DataManager {
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

            void read_data(const std::string& filename, const std::string& commands_tree_file);
    };

}