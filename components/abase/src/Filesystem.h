#pragma once

#include "Environment.h"

/// @brief Header file containing file system operations and path manipulation utilities
/// 
/// This header provides a set of functions to:
/// - Check file permissions (readable/writable)
/// - Manipulate file paths (splitting, getting executable paths)
/// - File system operations (listing files, removing files/directories)
/// - Path resolution (executable and application paths)
///
/// The functions in this header file are designed to work with standard string paths
/// and provide common file system operations in a platform-independent way.
///
/// Example usage:
/// @code
/// std::string execPath = abase::getExecutablePath("myapp");
/// auto [head, tail] = abase::splitPath("/path/to/file");
/// bool canWrite = abase::is_file_writable("config.txt");
/// @endcode

namespace abase {
    
    /// @brief Check if the file is readable by the user
    /// @param filename the file name
    /// @return readable status
    bool is_file_readable(const std::string& filename);
    /// @brief Check if the file is writable by the user
    /// @param filename the file name
    /// @return writable status
    bool is_file_writable(const std::string& filename);

    /// @brief Return the absolute path of a file
    /// @param cmd relative path to a file
    /// @return Absolute path to the file
    std::string getAbsolutePath(const std::string& cmd);
    
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

    /// @brief Split a path in a head and an extension. The extension is the last part of the path after the last dot.
    /// @param a_path the path to split
    /// @return A pair of string that give the base name and the extension
    std::pair<std::string, std::string> splitExt(const std::string& a_path);

    /// @brief Remove a file from the file system
    /// @param filename the file to remove
    /// @return removal status
    bool remove(const std::string& filename);

    /// @brief Remove a folder and recursively its contained.
    /// @param foldername the folder to remove
    /// @return removal status
    bool remove_all(const std::string& foldername);


}