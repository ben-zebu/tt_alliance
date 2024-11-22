#pragma once

#include <iostream>
#include <filesystem>
#include <limits.h>
#include <string>
#include <utility>

namespace abase {
    
    /// @brief Return the path of the executable file
    /// @param cmd Executable file (relative path)
    /// @return Absolute path of the executable file
    std::string getExecutablePath(const std::string& cmd);
    
    /// @brief Return the main path of the application
    /// @param cmd Executable file (relative path)
    /// @return Absolute path of the application
    std::string getAppPath(const std::string& cmd);

    /// @brief Split a path in a head and a tail. The tail is the last folder level. The head is the absolute path
    /// to the tail folder. 
    /// @param a_path the path to split
    /// @return A pair of string that give the head and the tail
    std::pair<std::string, std::string> splitPath(const std::string& a_path);
}