#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <string>
# include <cctype>
# include <fstream>
# include <sstream>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/time.h>

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
std::string num_to_string(int num);
long long   str_to_ll(const char* str);
void        getlineSkipDelemeter(std::istringstream& stream_file_contents, std::string& buff, char c);
int         isFileOrDirectory(const char* path);
int         hexToDecimal(const char buf[], int size);
bool        isDirectory(const std::string& path);
long long   start_time_inspect(long long previous_time, std::string &test_syntax);
long long   end_time_inspect(long long previous_time, std::string &test_syntax);

#endif