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
        throw (std::runtime_error("Invalid config file contents [semicolon error [" + res_str + "]]"));
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

int isFileOrDirectory(const char* path)
{
    // std::cout << "path<" << path << ">path" << std::endl;
    struct stat fileInfo;
    // std::cout << "path: " << path << "\n";
    if (stat(path, &fileInfo) != 0) {
        // stat 호출이 실패한 경우
        return (FILE_NOT_FOUND);
    }

    if (S_ISREG(fileInfo.st_mode)) {
        // 일반 파일인 경우
        return IS_FILE;
    } else if (S_ISDIR(fileInfo.st_mode)) {
        // 디렉토리인 경우
        return IS_DIR;
    }
    return (IS_DIR);
}

int hexToDecimal(const char buf[], int size)
{
    int result = 0; // 결과 값을 저장할 변수
    for (int i = 0; i < size; ++i) {
        result *= 16; // 16진수이므로, 이전 결과에 16을 곱함

        if (isdigit(buf[i])) { // 숫자인 경우
            result += buf[i] - '0';
        } else if (isalpha(buf[i])) { // 알파벳인 경우 (A-F, a-f)
            char upper = toupper(buf[i]); // 대문자로 변환
            result += 10 + (upper - 'A');
        } else {
            // 잘못된 문자가 입력되었을 때의 처리
            std::cerr << "Invalid hexadecimal character: " << buf[i] << std::endl;
            return -1; // 오류를 나타내는 값 반환
        }
    }
    return result;
}

void reqPrint(char *buf, int n)
{
    int i = 0;
    while (i < n)
    {
        if (buf[i] == '\r')
            write (1, "\\r", 2);
        else
            write (1, &buf[i], 1);
        i++;
    }
}

bool existMethod(std::string method, std::vector<std::string> accessMethod)
{
    for(size_t i = 0; i < accessMethod.size(); i++)
    {
        if (accessMethod[i] == method)
            return (true);
    }
    return (false);
}
