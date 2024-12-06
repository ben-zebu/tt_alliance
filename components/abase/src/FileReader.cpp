#include <regex>

#include "Environment.h"
#include "FileReader.h"

using namespace abase;

//
// FileLineFilter
//
std::string FileLineFilter::comment_regex(const std::string& delimiters) {
    std::string regexStr = "["; 
    for (char ch : delimiters) regexStr += char_regex(ch); 
    regexStr += "].*"; 
    return regexStr;
}

std::string FileLineFilter::range_regex(const std::string& delimiters) {
    return "";
}

std::vector<std::string> FileLineFilter::filter(const std::string& line) {
    // Suppress comments in the line
    std::regex CommentRegex(comment_regex(COMMENT_DELIMITER));
    std::string line_without_comments = std::regex_replace(line, CommentRegex, "");
    line_without_comments = str::trim(line_without_comments);
    if (line_without_comments.front() == '*') line_without_comments.erase(0, 1);

    return str::split(line_without_comments);
}

//
// FileReader
//

FileReader::FileReader(const std::string& input_name) : filename(input_name) {
    input.open(input_name);
    if (!input.is_open()) {
        error(translate("ERROR_OPEN_FILE", input_name));
    }
}

std::string FileReader::get_word() {
    // fill buffer if empty
    if (buffer.empty()) buffer_update();
    // end of file
    if (buffer.empty()) return "";
    // standard case
    return buffer[word_rk];
}
                   
void FileReader::move() {
    word_rk++;
    if (word_rk >= buffer.size()) buffer_update();
}

 void FileReader::move_line() {
    word_rk = buffer.size();
    move();
 }

void FileReader::buffer_update() {
    std::lock_guard<std::mutex> lock(mutex);

    // clear buffer and line
    buffer.clear();
    line.clear();
    
    // read next not empty line
    while(std::getline(input, line)) {
        buffer = FileLineFilter::filter(line);
        if (!buffer.empty()) break;
    }
    word_rk = 0;
}

std::string FileReader::context_error() const {
    std::cout << "Error in file " << filename << " at line " << line << std::endl;
    return translate("ERROR_FILE_FOOTER", {filename, line});
}
