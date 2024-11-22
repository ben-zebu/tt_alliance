#include <iostream>
#include <filesystem>
#include <limits.h>
#include <string>
#include <utility>

#include "LocalPath.h"

    
std::string abase::getExecutablePath(const std::string& cmd) {
    return std::filesystem::absolute(cmd);
}

std::string abase::getAppPath(const std::string& cmd) {
    std::string exe_path = abase::getExecutablePath(cmd);

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

