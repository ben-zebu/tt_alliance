#include "CommandsCollector.h"
#include "Filesystem.h"

#include "FatigueLawCollector.h"

using namespace adata;

std::shared_ptr<FatigueLaw> FatigueLawCollector::create_law(const std::string& type) const {
    if (type == "TABULAR_FATIGUE_LAW") return std::make_shared<TabularFatigueLaw>();
    if (type == "POWER_FATIGUE_LAW") return std::make_shared<PowerFatigueLaw>();
    if (type == "POLYNOMIAL_FATIGUE_LAW") return std::make_shared<PolynomialFatigueLaw>();
    return nullptr;
}

void FatigueLawCollector::set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext) {
    std::string name = command->get_name();

    // create the fatigue law object
    std::shared_ptr<FatigueLaw> new_law = create_law(name);
    if (new_law == nullptr) {
        input_error(translate("FATIGUE_LAW_TYPE_UNKNOWN", name));
    }

    // initialize the fatigue law object
    new_law->init(command);
    new_law->verify(filecontext);

    // check if the law already exists
    for (const auto& law : laws) {
        if (law->is_same_law(*new_law)) {
            std::string msg = translate("FATIGUE_LAW_CONFLICT", new_law->get_id());
            file_input_error(msg, filecontext);
        }
    }

    // add the fatigue law to the collection
    laws.push_back(new_law);
}


void FatigueLawCollector::read_data(const std::string& filename, const std::string& commands_tree_file) {
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

std::shared_ptr<FatigueLaw> FatigueLawCollector::get_law(const std::string& law_id, const std::string& code="", 
                                                         const std::string& edition="") const { 
    for (const auto& law : laws) {
        if (law->get_id() == law_id && law->support_code(code, edition)) return law;
    }
    return nullptr;
}

