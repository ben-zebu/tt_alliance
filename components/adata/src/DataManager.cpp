#include "CommandsCollector.h"
#include "FileReader.h"

#include "DataManager.h"

using namespace adata;



void DataManager::read_data(const std::string& filename, const std::string& commands_tree_file) {

    // create commands factory based of external file
    abase::CommandsCollector commands_reader;
    commands_reader.loadCommandsFromFile(commands_tree_file);
    std::vector<std::string> commands_names = commands_reader.get_commands_names();

    // create the file reader
    abase::FileReader reader(filename);

    // read the input file based on each command reader
    std::size_t cumul_status = 0;
    while (reader.get_word().size() > 0 && cumul_status == 0) {
        cumul_status = 1;
        for (const auto& name : commands_names) {
            auto command = commands_reader.get_command(name);
            std::size_t status = command->read_input(reader);
            if (status == 0) {
                // add the command to the data manager
                std::cout << "Command " << name << " read" << std::endl;
            }
            cumul_status *= status;
        }
    }

}