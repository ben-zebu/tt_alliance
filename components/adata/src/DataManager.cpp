#include "DataManager.h"

using namespace adata;

void DataManager::set_data(const std::shared_ptr<abase::BaseCommand>& command, const std::string& filecontext) {
    std::string name = command->get_name();

    if (name == "FEM_FILE") {
        fem_interface.init(command);
        fem_interface.verify(filecontext);
    } 

    if (name == "CODE") {
        description.init(command);
        description.verify(filecontext);
    }

    if (name == "PLATE") {
        plate.init(command, description.category);
        plate.verify(filecontext);
    }

    if (name == "TORSOR") {
        torsor.init(command);
        torsor.verify(filecontext);
    }

    if (name == "LOADSTEP") {
        ProblemLoadstep loadstep;
        loadstep.init(command, torsor.cards.size());
        loadstep.verify(filecontext);
        loadsteps.push_back(loadstep);
    }

    if (name == "TRANSIENT") {
        ProblemTransient transient;
        transient.init(command, transients.size(), loadsteps.size());
        transient.verify(filecontext);
        transients.push_back(transient);
    }

    if (name == "TABLE") {
        ProblemTable table;
        table.init(command, tables.size());
        table.verify(filecontext);
        tables.push_back(table);
    }
}

std::vector<std::string> DataManager::read_title(abase::FileReader& reader, const abase::CommandsCollector& collector) {
    std::vector<std::string> title;
    std::string key = reader.get_word();
    while (key.size() > 0 && !collector.is_command_name(key)) {
        title.push_back( str::trim(reader.get_line()) );
        reader.move_line();
        key = reader.get_word();
    }
    return title;
}

void DataManager::read_special_line(const std::string& command_name, abase::FileReader& reader, 
                                    const abase::CommandsCollector& collector) {
    if (command_name == "CODE") {
        description.titles = read_title(reader, collector);
    }
}

void DataManager::verify() const {
    /*
    // check if cross-sections are defined
    if (sections.size() == 0) {
        std::string msg = translate("ERROR_MISSING_SECTION");
        input_error(msg);
    }
    */

    // check if transients are defined for category 2
    if (description.category == 2 && transients.size() == 0) {
        std::string msg = translate("ERROR_MISSING_TRANSIENT");
        input_error(msg);
    }

    // check unicities of transient names
    if (transients.size() > 0) {
        std::vector<std::string> names;
        for (const auto& transient : transients) {
            if (std::find(names.begin(), names.end(), transient.name) != names.end()) {
                std::string msg = translate("ERROR_DUPLICATE_TRANSIENT_NAME", transient.name);
                input_error(msg);
            } else {
                names.push_back(transient.name);
            }
        }
    }

}
