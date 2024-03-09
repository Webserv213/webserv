# include "Utils.hpp"
# include <fstream>
# include <iostream>
# include <string>
# include <sstream>
# include <sys/stat.h>

void   printCharVector(std::vector<char>& vector_char)
{
    for (size_t i = 0; i < vector_char.size(); i++)
        std::cout << vector_char[i];
}

std::vector<char>   stringToCharVector(std::string& str)
{
    std::vector<char> vector_char;
    for (size_t i = 0; i < str.size(); i++)
        vector_char.push_back(str[i]);
    return (vector_char);
}

std::string charVectorToString(std::vector<char>& vector_char)
{
    std::string str;

    for (size_t i = 0; i < vector_char.size(); i++)
        str += vector_char[i];
    return (str);
}

std::string getFileContents(std::string input_file_name)
{
    std::ifstream input_file_stream(input_file_name.c_str());
    std::string buff, file_contents;

	if (!input_file_stream.is_open())
        throw (std::runtime_error("Failed open confuration file"));

    while (std::getline(input_file_stream, buff))
		file_contents.append(buff);
    return (file_contents);
}

std::string strTrim(std::string &str, char c)
{
    std::string     res_str;
    long long       start = 0;
    long long       end = 0;

    while (start < (long long)str.size())
    {
        if (str[start] != c)
            break ;
        start++;
    }
    end = str.size() - 1;
    while (end >= 0)
    {
        if (str[end] != c)
            break ;
        end--;
    }
    while (start <= end)
    {
        res_str += str[start];
        start++;
    }
    return (res_str);
}

std::string checkSemicolon(std::string &str)
{
    std::string res_str;

    res_str = strTrim(str, ' ');
    if (*(res_str.rbegin()) != ';')
        throw (std::runtime_error("Invalid config file contents [semicolon error]"));
    res_str.pop_back();
    return (res_str);
}

void getlineSkipDelemeter(std::istringstream& stream_file_contents, std::string& buff, char c)
{
    while (std::getline(stream_file_contents, buff, c))
    {
        if (buff != "")
            break ;
    }
}

std::string mySubstr(std::string::iterator start, std::string::iterator end)
{
    std::string result_str;

    for (std::string::iterator it = start; it != end; ++it)
    {
        result_str += *it;
    }

    return result_str;
}

bool isFileOrDirectory(const char* path)
{
    struct stat fileInfo;
    if (stat(path, &fileInfo) != 0) {
        // stat 호출이 실패한 경우
        throw(std::runtime_error("file error(isFileOrDirectory)"));
    }

    if (S_ISREG(fileInfo.st_mode)) {
        // 일반 파일인 경우
        return true;
    } else if (S_ISDIR(fileInfo.st_mode)) {
        // 디렉토리인 경우
        return false;
    }
    return (false);
}
