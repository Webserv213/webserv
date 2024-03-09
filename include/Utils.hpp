#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <vector>

void                printCharVector(std::vector<char>& vector_char);
std::vector<char>   stringToCharVector(std::string& str);
std::string         charVectorToString(std::vector<char>& vector_char);
std::string         getFileContents(std::string input_file_name);
std::string         strTrim(std::string &str, char c);
std::string         checkSemicolon(std::string &str);
void                getlineSkipDelemeter(std::istringstream& stream_file_contents, std::string& buff, char c);
std::string         mySubstr(std::string::iterator start, std::string::iterator end);
bool				isFileOrDirectory(const char* path);


#endif
