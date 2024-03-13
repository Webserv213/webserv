# include "Http.hpp"
# include <fcntl.h>
# include <iostream>
# include <string>
# include <unistd.h>
# include <sstream>
# include <stack>
# include <fstream>

Http::Http(void)
{
    close_http_bracket = -1;
}

Http::~Http(void)
{

}

void Http::pushBackServerBlock(Server &server)
{
    server_block_.push_back(server);
}

std::vector<Server>& Http::getServer()
{
    return (server_block_);
}

Server Http::setServerBlock(std::istringstream& stream_file_contents)
{
    std::string     buff;
    Server          new_server;

    // 서버 블록 여는 괄호 체크
    getlineSkipDelemeter(stream_file_contents, buff, ' ');
    if (buff != "{")
        throw (std::runtime_error("Invalid config file contents [server's open bracket error]"));



    // 서버 블록 데이터 저장
    while (std::getline(stream_file_contents, buff, ' '))
    {
        if (buff[0] == '}')
        {
            if (buff.size() != 1 && buff[1] == '}')
                close_http_bracket = HTTPBLOCK_CLOSE_OK;
            return (new_server);
        }
        else if (buff == "listen")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            new_server.setListenPort(buff);
        }
        else if (buff == "server_name")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            new_server.setServerName(buff);
        }
        else if (buff == "error_page")
        {
            std::string error_page_path;
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            getlineSkipDelemeter(stream_file_contents, error_page_path, ' ');
            error_page_path = checkSemicolon(error_page_path);
            new_server.setErrorPage(buff, error_page_path);
        }
        else if (buff == "autoindex")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            if (buff == "on")
                new_server.setAutoIndex(true);
            else
                new_server.setAutoIndex(false);    
        }
        else if (buff == "root")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            new_server.setRoot(buff);
        }
        else if (buff == "index")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            new_server.setIndex(buff);
        }
        else if (buff == "location")
        {
            if (new_server.getLocation().size() == 0)
                new_server.createDefaultLocation();

            Location new_location = new_server.setLocationBlock(stream_file_contents);
            if (new_location.getUrlPostfix().size() == 0)    // '/'인 경우 [0]번째에 넣어주기
                new_server.fixLocation(0, new_location);
            else
                new_server.pushBackLocationBlock(new_location);
        }
    }

    if (new_server.getLocation().size() == 0)
        new_server.createDefaultLocation();

    throw (std::runtime_error("Invalid config file contents [server's close bracket error]"));
    return (new_server);
}

void Http::setHttpBlock(std::istringstream& stream_file_contents)
{
    std::string buff;

    std::getline(stream_file_contents, buff, ' ');
    if (buff != "http")
        throw (std::runtime_error("Invalid config file contents [http block name error]"));
    std::getline(stream_file_contents, buff, ' ');
    if (buff != "{")
        throw (std::runtime_error("Invalid config file contents [http's open bracket error]" + buff));

    // server 블록 유효성 확인
    while (std::getline(stream_file_contents, buff, ' '))
    {
        if (buff == "}")
            return ;
        else if (buff == "server")
        {
            Server new_server = setServerBlock(stream_file_contents);
            this->pushBackServerBlock(new_server);
        }
    }
    
    // http 블록 유효성 확인 : 닫는 괄호 확인
    if (close_http_bracket != HTTPBLOCK_CLOSE_OK)
        throw (std::runtime_error("Invalid config file contents [http's close bracket error]"));
}

void Http::setWebserv(int argc, char **argv)
{
    std::string file_contents;
    std::istringstream stream_file_contents;

    // conf file 내용 가져오기
    if (argc > 2)
        throw (std::runtime_error("Invalid argc"));
    if (argc == 2)
        file_contents = getFileContents(argv[1]);
    else
        file_contents = getFileContents(DEFAULT_CONF_FILE_PATH);

    stream_file_contents.str(file_contents);    // string을 stream객체로 변환
    
    // setting http 블록 
    setHttpBlock(stream_file_contents);
}