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

    // Regex expression to split the line into words
    std::regex SplitRegex(R"((["'][^"']*["'])|(\S+))");
    auto wordsBegin = std::sregex_iterator(line_without_comments.begin(), line_without_comments.end(), SplitRegex);
    auto wordsEnd = std::sregex_iterator();

    // Process each word
    std::vector<std::string> result;
    for (auto it = wordsBegin; it != wordsEnd; ++it) {
        std::string word = it->str();
        // Remove leading and trailing spaces
        if (word.front() == '"' || word.front() == '\'') word.erase(0, 1);
        if (word.back() == '"' || word.back() == '\'') word.pop_back();

        if (!word.empty()) result.push_back(word);        
    }
    return result;
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

void FileReader::buffer_update() {
    std::lock_guard<std::mutex> lock(mutex);
    line.clear();
    while(std::getline(input, line)) {
        buffer = FileLineFilter::filter(line);
        if (!buffer.empty()) break;
    }
    word_rk = 0;

    // case for end of file
    if (line.empty()) buffer.clear();
}

std::string FileReader::context_error() const {
    return translate("ERROR_FILE_FOOTER", {filename, line});
}
