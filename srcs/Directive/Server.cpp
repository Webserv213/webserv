#include "Server.hpp"

Server::Server(void)
{
    root_ = "./var/www";
    autoindex_ = true;
    client_max_body_size_ = 10000;
    error_page_[404]="./var/error";

    listen_["127.0.0.1"] = 8081;
    server_name_="webserv.com";

    Location l;
    location_block_.push_back(l);
}

Server::~Server(void)
{
    
}

void Server::setLocation(Location &location)
{
    location_block_.push_back(location);
}

// getter
std::string Server::getRoot(void)
{
    return (root_);
}

bool Server::getAutoIndex(void)
{
    return (autoindex_);
}

unsigned long long Server::getClientMaxBodySize(void)
{
    return (client_max_body_size_);
}

std::string Server::getErrorPage(int idx)
{
    return (error_page_[idx]);
}

unsigned short Server::getListen(std::string ip)
{
    return (listen_[ip]);
}

std::string Server::getServerName(void)
{
    return (server_name_);
}

Location Server::getLocationBlock(int idx)
{
    return (location_block_[idx]);
}

// util
int Server::findLocationBlock(std::string url)   // 도메인 뒤에 url로 넘겨주도록
{
    for(size_t i = 0; i < location_block_.size(); i++)
    {
        if (location_block_[i].getPathPostfix() == url)
        {
            return (i);
        }
    }
    return (-1);
}
