#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

std::string modifyFile(const char* filename, std::string value) {
    std::ifstream file(filename); // 파일을 읽기 모드로 열기
    if (!file.is_open()) {
        std::cerr << "파일을 열 수 없습니다." << std::endl;
        return (NULL);
    }

    // 파일 내용 읽기
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    size_t index = content.find("{$name}");
    if (index != std::string::npos) {
        content.replace(index, strlen("{$name}"), value);
    }

    return (content);
}

int main(int argc, char **argv, char **env) {
    const char* filename = "./var/www/session/displayName.html"; // 파일 이름
    std::string body;

    (void)argv;
    (void)env;

    if (argc != 2)
        // error
        ;
    
    std::string value;
    int n = 0;
    char buf[32767];
    do
    {
        n = read(0, buf, 32767);
        if (n < 0)
        {
            write(2, "웹서브에서 cgi로 보내는 바디 읽기 실패", 54);
            exit(1);
        }
        value.append(buf, n);
    } while (n != 0);

    body = modifyFile(filename, value);

    std::string response = "200 OK\r\ntext/html; charset=utf-8\r\n\r\n";
    response += body;

    if (write(1, &response.c_str()[0], response.size()) < 0)
    {
        write(2, "cgi 데이터 전달 실패", 28);
        std::cout << "cgi에서 웹서브로 데이터 전달 실패" << std::endl;
        exit(1);
    }
    close(1);
    return (0);
}