# include "Http.hpp"
# include <fcntl.h>
# include <iostream>
# include <string>

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
void Http::setWebserv(int argc, char **argv)
{
    int fd = -1;
    int buf[BUFFER_SIZE];

    if (argc > 2)
        throw (std::runtime_error("argv error"));
    if (argc == 2)
        fd = open(argv[1], O_RDONLY);
    else
        fd = open(DEFAULT_CONFIG_PATH, O_RDONLY);
    if (fd < 0)
        throw (std::runtime_error("config file open error"));

    while (std::getline())
    {

    }
}
