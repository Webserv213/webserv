#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <vector>
# include <cctype> // isdigit, isalpha 함수 사용을 위함
# include <unistd.h>

enum FileCheck
{
    IS_FILE,
    IS_DIR,
    FILE_NOT_FOUND
};

enum AutoIndexCheck
{
	ON,
	OFF
};

void                printCharVector(std::vector<char>& vector_char);
std::vector<char>   stringToCharVector(std::string& str);
std::string         charVectorToString(std::vector<char>& vector_char);
std::string         getFileContents(std::string input_file_name);
std::string         strTrim(std::string &str, char c);
std::string         checkSemicolon(std::string &str);
void                getlineSkipDelemeter(std::istringstream& stream_file_contents, std::string& buff, char c);
std::string         mySubstr(std::string::iterator start, std::string::iterator end);
int 				isFileOrDirectory(const char* path);
int                 hexToDecimal(const char buf[], int size);
void                reqPrint(char *buf, int n);
bool                existMethod(std::string method, std::vector<std::string> accessMethod);
void                printCharVectorCRLF(std::vector<char>& vector_char);

#endif
