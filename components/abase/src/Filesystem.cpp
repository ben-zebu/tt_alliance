#include <filesystem>
#include <utility>

#include "Filesystem.h"

namespace fs = std::filesystem;

bool abase::is_file_readable(const std::string& filename) {
    // check if the file exists and is a regular file
    if (!fs::exists(filename) || !fs::is_regular_file(filename) ) {
        return false;
    }

    // get the status and the permission of the file
    fs::file_status filestatus = fs::status(filename);
    fs::perms p = filestatus.permissions();
    return ((p & fs::perms::owner_read) != fs::perms::none) ||
           ((p & fs::perms::group_read) != fs::perms::none) ||
           ((p & fs::perms::others_read) != fs::perms::none);
}

bool abase::is_file_writable(const std::string& filename) {
    // check if the file exists and is a regular file
    if (!fs::exists(filename) || !fs::is_regular_file(filename) ) {
        return false;
    }

    // get the status and the permission of the file
    fs::file_status filestatus = fs::status(filename);
    fs::perms p = filestatus.permissions();
    return ((p & fs::perms::owner_write) != fs::perms::none) ||
           ((p & fs::perms::group_write) != fs::perms::none) ||
           ((p & fs::perms::others_write) != fs::perms::none);
}


std::string abase::getAbsolutePath(const std::string& cmd) {
    return fs::absolute(cmd);
}

std::string abase::getAppPath(const std::string& cmd) {
    std::string exe_path = abase::getAbsolutePath(cmd);

    auto [path, exe] = abase::splitPath(exe_path);
    auto [project, exe_dir] = abase::splitPath(path);

    return project;
}

std::pair<std::string, std::string> abase::splitPath(const std::string& a_path) {
    size_t lastSeparator = a_path.find_last_of("/\\");
    
    std::string head = a_path.substr(0, lastSeparator);
    std::string tail = a_path.substr(lastSeparator + 1);
    
    return {head, tail};
}

std::vector<std::string> abase::getFilesbyExtension(const std::string& directory, const std::string& ext) {
    std::vector<std::string> files;
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.path().extension() == ext) {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

std::pair<std::string, std::string> abase::splitExt(const std::string& a_path) {
    size_t lastSeparator = a_path.find_last_of(".");
    
    std::string head = a_path.substr(0, lastSeparator);
    std::string tail = a_path.substr(lastSeparator + 1);
    
    return {head, tail};
}

bool abase::remove(const std::string& filename) {
    if (!fs::exists(filename)) return false;
    return fs::remove(filename);
}

bool abase::remove_all(const std::string& foldername) {
    if (!fs::exists(foldername)) return false;
    std::uintmax_t count = fs::remove_all(foldername);
    return (count > 0) ? true : false;
}

bool abase::create_directory(const std::string& foldername) {
    return fs::create_directory(foldername);
}