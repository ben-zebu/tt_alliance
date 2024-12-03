
#include "Filesystem.h"
#include "OutputResume.h"


OutputResume::OutputResume(const std::string& filename) : input_filename(filename) {
    suppress_previous_results();
    logFile.open(output_resume());
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
