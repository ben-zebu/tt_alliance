#include "Filesystem.h"
#include "OutputResume.h"
#include "System.h"

void OutputResume::init(const std::string& filename) {
    input_filename = filename;

    // suppress previous results files
    suppress_previous_results();

    // open output file and create results folder
    logFile.open(output_resume());
    create_results_folder();

    // write header
    write_header();
}

std::string OutputResume::get_basename() {
    std::pair<std::string,std::string> path_pair = abase::splitExt(input_filename);
    return path_pair.first;
}

std::string OutputResume::output_resume() {
    std::string basename = get_basename();
    return basename + std::string(RESUME_EXTENSION);
};

std::string OutputResume::output_listing() {
    std::string basename = get_basename();
    return basename + std::string(LISTING_EXTENSION);
};

std::string OutputResume::output_folder() {
    std::string basename = get_basename();
    return basename + std::string(LOG_DIR_EXTENSION);
};

void OutputResume::suppress_previous_results() {
    abase::remove(output_resume());
    abase::remove(output_listing());
    abase::remove_all(output_folder());
}

void OutputResume::create_results_folder() {
    std::string folder = output_folder();
    if (!abase::create_directory(folder)) {
        error( translate("ERROR_CREATE_DIRECTORY", folder) );
    }
}

void OutputResume::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    logFile.write(message);
}

void OutputResume::write_header() {

    std::string program = get_parser_value<std::string>("project");
    std::string version = get_parser_value<std::string>("version");
    std::string platform = get_parser_value<std::string>("system");

    std::string date = abase::current_date();
    std::string hostname = abase::get_hostname();

    std::string header_line(columns, '=');
    std::string header = translate("PROGRAM", program) + "\n";
    header += translate("VERSION", version) + "\n" + "\n";
    header += translate("HOST_AND_DATE", {date, hostname + ", " + platform});

    write(header_line);
    write(header);
    write(header_line + "\n");

}