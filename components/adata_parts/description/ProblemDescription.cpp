#include "ProblemDescription.h"

using namespace adata;

void ProblemDescription::init(const std::shared_ptr<abase::BaseCommand>& command) {
    command->get_value(code_type);
    code_type = str::uppercase(code_type);

    abase::get_child_value(command, "VERSION", code_edition);
    abase::get_child_value(command, "CATEGORY", category);
    abase::get_child_value(command, "NS", earthquakes);
    abase::get_child_value(command, "LANGUAGE", language);
    abase::get_child_value(command, "EDITION", edition);
    abase::get_child_value(command, "UNITS", units);
}

void ProblemDescription::verify(const std::string& filecontext) const {
    // Version control is only required for catgory 2
    if (category != 2) return;

    // check if the code edition is defined in category 2
    if (code_edition.empty() ) {
        file_input_error(translate("CODE_EDITION_MISSING"), filecontext);
    }

    // check if the code edition is available for the code type
    std::string versions;
    if (code_type == "RCCM") versions = get_parser_value<std::string>("rccm_versions");
    if (code_type == "ASME") versions = get_parser_value<std::string>("asme_versions");
    versions = str::replace(versions, ",", " ");

    bool available_version = false;
    for (const auto& version : str::split(versions)) {
        if (code_edition != version) continue;
        available_version = true;
        break;
    }

    if (!available_version) {
        std::string msg = translate("CODE_EDITION_UNKNOWN", {code_edition, code_type});
        file_input_error(msg, filecontext);
    }

}