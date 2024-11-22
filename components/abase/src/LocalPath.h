#pragma once

#include <iostream>
#include <filesystem>
#include <limits.h>
#include <string>
#include <utility>
#include <vector>

namespace abase {
    
    /// @brief Check if the file is readable by the user
    /// @param filename the file name
    /// @return readable status
    bool is_file_readable(const std::string& filename);
    /// @brief Check if the file is writable by the user
    /// @param filename the file name
    /// @return writable status
    bool is_file_writable(const std::string& filename);

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

    /// @brief Return the list of files with a specific extension and contained in a folder
    /// @param directory the folder used to search the files with an extension
    /// @param ext the expected extension
    /// @return the list of files
    std::vector<std::string> getFilesbyExtension(const std::string& directory, const std::string& ext);

}