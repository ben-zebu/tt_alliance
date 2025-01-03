#include "CommandsCollector.h"
#include "FileReader.h"

#include "DataCollector.h"

using namespace abase;

void DataCollector::read_data(const std::string& filename, const std::string& commands_tree_file) {

    // create commands factory based of external file
    abase::CommandsCollector commands_reader;
    commands_reader.loadCommandsFromFile(commands_tree_file);
    std::vector<std::string> commands_names = commands_reader.get_commands_names();

    // create the file reader
    abase::FileReader reader(filename);

    // read the input file based on each command reader
    std::size_t cumul_status = 0;
    bool end_of_file = false;
    while (reader.get_word().size() > 0 && cumul_status == 0) {
        cumul_status = 1;

        for (const auto& name : commands_names) {
            auto command = commands_reader.get_command(name);
            std::size_t status = command->read_input(reader, commands_reader);
            if (status == 0) {
                std::string filecontext = reader.context_error();

                // add the command to the data manager
                set_data(command, filecontext);

                // special case for problem title
                read_special_line(command->get_name(), reader, commands_reader);

                // special case for end of input data
                if (command->get_name() == "RETURN") {
                    end_of_file = true;
                    break;
                }
            }
            cumul_status *= status;
            if (cumul_status == 0) break;
        }
    }

    // check if the end of file is reached
    if (end_of_file) return;

    // Error case when the current word is not found in the command list
    if (reader.get_word().size() > 0 && cumul_status == 1) {
        std::string error_msg = translate("ERROR_DATA_UNKNOWN_COMMAND", reader.get_word());
        file_input_error(error_msg, reader.context_error());
    }

}
