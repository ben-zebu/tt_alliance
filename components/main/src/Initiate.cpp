#include "ArgumentParser.h"
#include "GlobalTimer.h"
#include "LocalPath.h"

#include "Initiate.h"

constexpr std::string_view CONFIG_FILE = "/etc/alliance.conf";
constexpr std::string_view RESSOURCES_DIR = "/ressources";

/// @brief Load the configuration file
void load_configuration() {
    std::string app_path = get_parser_value<std::string>("application_path");
    abase::globalConfigParser.loadFromFile(app_path + std::string(CONFIG_FILE));
}

/// @brief Load all translations files
void load_translations() {
    std::string app_path = get_parser_value<std::string>("application_path");
    std::string lang = get_parser_value<std::string>("language");

    abase::globalTranslationManager.loadAllTranslations(app_path + std::string(RESSOURCES_DIR));
    abase::globalTranslationManager.setCurrentLanguage(lang);
}

/// @brief Parse the command line
/// @param argc number of arguments
/// @param argv list of arguments
void parse_commands_line(int argc, char* argv[]) {
    adata::ArgumentParser parser(argc, argv);
} 

void init::start(int argc, char* argv[]) {
    // start the global timer
    start_timer("global_timer");

    // set the path of the executable for future purpose
    std::string app_path = abase::getAppPath(argv[0]);
    set_parser_value("application_path", app_path);

    // initialization process
    load_configuration();
    parse_commands_line(argc, argv);
    load_translations();
}
