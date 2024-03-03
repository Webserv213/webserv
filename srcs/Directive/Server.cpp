#include "Server.hpp"

Server::Server(void)
{
    root_ = "./var/www";
    autoindex_ = true;
    client_max_body_size_ = 10000;
    error_page_["404"] = "./var/error";

    // listen_["127.0.0.1"] = 8082;
    server_name_="webserv.com";

    Location l;
    location_block_.push_back(l);
}

Server::~Server(void)
{

}

void Server::setLocationValue(std::string &str, Location &new_location)
{
    std::istringstream  stream_config_contents(str);
    std::string         get_str;

    std::getline(stream_config_contents, get_str, ' ');
    if (get_str == "location") {
        std::getline(stream_config_contents, get_str, ' ');
        new_location.setUrlPostfix(get_str);
        std::getline(stream_config_contents, get_str, '\n');
        if (get_str != "{")
            throw (std::runtime_error("Invalid config file contents [location start bracket]"));
    }
    else if (get_str == "root")
    {
        std::getline(stream_config_contents, get_str, '\n');
        new_location.setRoot(get_str);
    }
    else if (get_str == "autoindex")
    {
        std::getline(stream_config_contents, get_str, '\n');
        if (get_str == "on")
            new_location.setAutoIndex(true);
        else if (get_str == "off")
            new_location.setAutoIndex(false);
        else
            throw (std::runtime_error("Invalid config file contents [Invalid autoindex value]"));
    }
    else if (get_str == "error_page")
    {
        std::getline(stream_config_contents, get_str, '\n');
        std::string get_path;
        std::getline(stream_config_contents, get_path, '\n');
        new_location.setErrorPage(get_str, get_path);
    }

    else if (get_str == "client_max_body_size")
    {
        std::getline(stream_config_contents, get_str, '\n');
        new_location.setClientMaxBodySize(std::atoll(get_str.c_str())); // c++11 함수 수정해야함.
    }
}

// setter
void Server::setRoot(std::string &root)
{
    root_ = root;
}

void Server::setAutoIndex(bool flag)
{
    autoindex_ = flag;
}

void Server::setClientMaxBodySize(unsigned long long client_max_body_size)
{
    client_max_body_size_ = client_max_body_size;
}

void Server::setErrorPage(std::string error_code, std::string &path)
{
    error_page_[error_code] = path;
}

void Server::setListenPort(std::string &listen_port)
{
    listen_port_ = std::atoi(listen_port.c_str());
}

void Server::setServerName(std::string &server_name)
{
    server_name_=server_name;
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

std::string Server::getErrorPage(std::string error_code)
{
    return (error_page_[error_code]);
}

unsigned short Server::getListenPort()
{
    return (listen_port_);
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
        if (location_block_[i].getUrlPostfix() == url)
        {
            return (i);
        }
    }
    return (-1);
}
