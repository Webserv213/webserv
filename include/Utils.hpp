#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <cctype>
# include <fstream>
# include <sstream>
# include <unistd.h>
# include <sys/stat.h>


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

std::string getFileContents(std::string input_file_name);
std::string strTrim(std::string &str, char c);
std::string checkSemicolon(std::string &str);
void        getlineSkipDelemeter(std::istringstream& stream_file_contents, std::string& buff, char c);
int         isFileOrDirectory(const char* path);
int         hexToDecimal(const char buf[], int size);

#endif
