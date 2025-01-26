#include <ctime>
#include <unistd.h>

#include "System.h"

std::string abase::current_date() {
    std::time_t now = std::time(0);
    std::tm* time = std::localtime(&now);

    char date_string[80];
    std::string language = get_parser_value<std::string>("language");
    if (language == "en") {
        std::strftime(date_string, sizeof(date_string), "%Y/%m/%d", time);
    } else {
        std::strftime(date_string, sizeof(date_string), "%d/%m/%Y", time);
    }
    
    return std::string(date_string);
}

std::string abase::get_hostname() {
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    return std::string(hostname);
}