# include "Utils.hpp"

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

int isFileOrDirectory(const char* path)
{
    struct stat fileInfo;

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

std::string num_to_string(int num)
{
    std::stringstream ss;
    ss << num;
    std::string str = ss.str();

    return (str);
}

long long str_to_ll(const char* str)
{
    long long num = std::atol(str);

    return (num);
}

bool isDirectory(const std::string& path)
{
    struct stat stat_buf;
    if (stat(path.c_str(), &stat_buf) != 0)
    {
        return false;
    }
    return S_ISDIR(stat_buf.st_mode);
}

// 시작 시간을 설정하는 함수
long long start_time_inspect(long long previous_time, std::string &test_syntax)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    // sec를 ms로 변환하고, us를 ms로 변환
    long long milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000; 
    long long time_diff = milliseconds - previous_time;

    previous_time = milliseconds;

    std::cout << test_syntax << " Start Time : " << time_diff << std::endl;

    return (previous_time);
}

// 시작 시간부터 종료 시간까지의 차이를 구하는 함수
long long end_time_inspect(long long previous_time, std::string &test_syntax)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    // sec를 ms로 변환하고, us를 ms로 변환
    long long milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
    long long time_diff = milliseconds - previous_time;

    previous_time = milliseconds;
    
    std::cout << test_syntax << " End Time : " << time_diff << std::endl;

    return (previous_time);
}