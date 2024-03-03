# include "Http.hpp"
# include <fcntl.h>
# include <iostream>
# include <string>
# include <unistd.h>
# include <sstream>
# include <stack>

Http::Http(void)
{

}

Http::~Http(void)
{

}

void Http::setServer(Server &server)
{
    server_block_.push_back(server);
}

std::vector<Server>& Http::getServer()
{
    return (server_block_);
}

void Http::setServerValue(std::string &str, Server &new_server)
{
    std::istringstream  stream_config_contents(str);
    std::string         get_str;

    std::getline(stream_config_contents, get_str, ' ');
    if (get_str == "listen") {
        std::getline(stream_config_contents, get_str, '\n');
        new_server.setListenPort(get_str);
    }
    else if (get_str == "server_name")
    {
        std::getline(stream_config_contents, get_str, '\n');
        new_server.setServerName(get_str);
    }
    else if (get_str == "root")
    {
        std::getline(stream_config_contents, get_str, '\n');
        new_server.setRoot(get_str);
    }
    else if (get_str == "error_page")
    {
        std::getline(stream_config_contents, get_str, '\n');
        std::string get_path;
        std::getline(stream_config_contents, get_path, '\n');
        new_server.setErrorPage(get_str, get_path);
    }
    else if (get_str == "autoindex")
    {
        std::getline(stream_config_contents, get_str, '\n');
        if (get_str == "on")
            new_server.setAutoIndex(true);
        else if (get_str == "off")
            new_server.setAutoIndex(false);
        else
            throw (std::runtime_error("Invalid config file contents [Invalid autoindex value]"));
    }
    else if (get_str == "client_max_body_size")
    {
        std::getline(stream_config_contents, get_str, '\n');
        new_server.setClientMaxBodySize(std::atoll(get_str.c_str())); // c++11 함수 수정해야함.
    }
}

void Http::setWebserv(int argc, char **argv)
{
    int fd = -1;
    char buf[BUFFER_SIZE];
    int n = 0;
    std::string config_contents, get_str;
    std::istringstream stream_config_contents;
    bool server_end_flag = false;
    bool location_end_flag = false;

    if (argc > 2)
        throw (std::runtime_error("argv error"));
    if (argc == 2)
        fd = open(argv[1], O_RDONLY);
    else
        fd = open(DEFAULT_CONFIG_PATH, O_RDONLY);
    if (fd < 0)
        throw (std::runtime_error("config file open error"));

    while (1)
    {
        n = read(fd, buf, sizeof(buf));
        if (n < 0)
            throw (std::runtime_error("config file read error"));
        if (n == 0)
            break ;
        buf[n] = '\0';
        config_contents += buf;
    }
    stream_config_contents.str(config_contents);
    std::getline(stream_config_contents, get_str, ' ');
    if (get_str != "http")
        throw (std::runtime_error("Invalid config file contents [http]"));
    std::getline(stream_config_contents, get_str, '\n');
    if (get_str != "{")
        throw (std::runtime_error("Invalid config file contents [http start bracket]"));
    while (std::getline(stream_config_contents, get_str, ' '))
    {
        if (get_str == "server")
        {
            int lcoation_idx = 0;
            std::getline(stream_config_contents, get_str, '\n');
            if (get_str != "{")
                throw (std::runtime_error("Invalid config file contents [server start bracket]"));
            Server new_server;
            while (std::getline(stream_config_contents, get_str, '\n'))
            {
                if (get_str.find("location") != std::string::npos)
                {
                    Location location;

                    // setLocationValue(get_str, location);
                    while (1)
                    {
                        // setLocationValue();
                    }

                }
            }
        }
    }
}