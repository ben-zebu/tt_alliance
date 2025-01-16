#include "Filesystem.h"

#include "ProblemFemInterface.h"

using namespace adata::parts;

void ProblemFemInterface::set_fem_results_file(const std::string& sequence) {
    std::vector<std::string> str_values = str::split(sequence);
    filename = "";
    for (const auto& wd : str_values) {
        std::string upper = str::uppercase(wd);
        if (upper.find("ASC") == 0) {
            filename += ".ASC";
        } else {
            filename += wd;
        }
    }
    if (filename.find(".") == std::string::npos) filename += ".TIT";

    // Try conversion in absolute path
    try { 
        filename = abase::getAbsolutePath(filename);
    } catch (const std::invalid_argument& e) {
        // nothing to do
    }
    
}

void ProblemFemInterface::init(const std::shared_ptr<abase::BaseCommand>& command) {
    std::string sequence;
    command->get_value(sequence);
    set_fem_results_file(sequence);
    std::cout << "fem filename: " << filename << std::endl;
}

void ProblemFemInterface::verify(const std::string& filecontext) const {
    // Try conversion in absolute path
    try { 
        std::string realpath = abase::getAbsolutePath(filename);
    } catch (const std::invalid_argument& e) {
        std::string msg = translate("ERROR_PATH_FEM_FILE", filename);
        file_input_error(msg, filecontext);
    }


    if (!abase::is_file_readable(filename)) {
        std::string msg = translate("ERROR_UNKNOWN_FEM_FILE", filename);
        file_input_error(msg, filecontext);
    }
}