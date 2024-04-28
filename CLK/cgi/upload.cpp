#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>

typedef struct info {
    std::string REQUEST_METHOD;
    std::string SERVER_PROTOCOL;
    std::string PATH_INFO;    // 파일 경로
    std::string CONTENT_LENGTH;
} env_struct;

int main(int argc, char **argv, char **env)
{
    (void)argv;
    env_struct env_list;

    if (argc != 2)
        // error
        ;
    
    int i = 0;
    while (env[i] != NULL)
    {
        std::string buf, key, value;
        std::string str_content = env[i];
        std::istringstream content(str_content);

        std::getline(content, key, '=');
        std::getline(content, value);

        if (key == "REQUEST_METHOD")
            env_list.REQUEST_METHOD = value;
        else if (key == "SERVER_PROTOCOL")
            env_list.SERVER_PROTOCOL = value;
        else if (key == "PATH_INFO")
            env_list.PATH_INFO = value;
        else if (key == "CONTENT_LENGTH")
            env_list.CONTENT_LENGTH = value;

        i++;
    }


    // 파일 open + write
    int fd;

    fd = open(env_list.PATH_INFO.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        write(2, "file open error\n", 18);
    }

    std::string body;
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
        body.append(buf, n);
    } while (n != 0);



    if (write(fd, &body.c_str()[0], body.size()) < 0)
    {
        write(2, "file에 쓰기 실패", 22);
        std::cout << env_list.PATH_INFO << ": file에 쓰기 실패" << std::endl;
        exit(1);
    }
    close(fd);



    std::string response = "200 OK\r\ntext/html; charset=utf-8\r\n\r\n";
    response += body;
    if (write(1, response.c_str(), response.size()) < 0)
    {
        write(2, "cgi 데이터 전달 실패", 28);
        std::cout << "cgi에서 웹서브로 데이터 전달 실패" << std::endl;
        exit(1);
    }
    close(1);
    return 0;
}