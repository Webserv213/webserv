#include "Server.hpp"

Server::Server(void)
{
    listen_port_ = 8080;
    server_name_="webserv.com";
    root_ = "./var/www/default";
    autoindex_ = false;
    client_max_body_size_ = -1;
    error_page_["404"] = "./var/www/error";
    index_ = "index.html";
}

Server::~Server(void)
{

}

Location Server::setLocationBlock(std::istringstream& stream_file_contents)
{
    std::string         buff;
    Location            new_location(*this);

    // 로케이션 블록은 http,서버 블록과 달리 여는 괄호 전에 value 처리해야함.
    getlineSkipDelemeter(stream_file_contents, buff, ' ');
    new_location.setUrlPostfix(buff);

    // 로케이션 블록 여는 괄호 체크
    getlineSkipDelemeter(stream_file_contents, buff, ' ');
    if (buff != "{")
        throw (std::runtime_error("Invalid config file contents [location's open bracket error]"));

    // 로케이션 블록 데이터 저장
    while (std::getline(stream_file_contents, buff, ' '))
    {
        if (buff == "}")
        {
            return (new_location);
        }
        else if (buff == "root")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            // std::cout << "buff: " << buff << "\n";
            new_location.setRoot(buff);
        }
        else if (buff == "autoindex")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            if (buff == "on")
                new_location.setAutoIndex(true);
            else
                new_location.setAutoIndex(false);
        }
        else if (buff == "client_max_body_size")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            new_location.setClientMaxBodySize(std::atoll(buff.c_str()));
        }
        else if (buff == "index")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            // std::cout << "buff: " << buff << "\n";
            new_location.setIndex(buff);
        }
        else if (buff == "access_method")
        {
            while (std::getline(stream_file_contents, buff, ' '))
            {
                if (buff.find(';') != std::string::npos)
                {
                    buff = checkSemicolon(buff);
                    new_location.setAccessMethod(buff);
                    break ;
                }
                else if (buff.find('}') != std::string::npos)
                {
                    throw (std::runtime_error("Invalid config file contents [semicolon error [access_method]]"));
                }
                else if (buff != "")
                {
                    new_location.setAccessMethod(buff);
                }
            }
        }
        else if (buff == "cgi_path")
        {
            getlineSkipDelemeter(stream_file_contents, buff, ' ');
            buff = checkSemicolon(buff);
            new_location.setCgiPath(buff);
        }
    }
    throw (std::runtime_error("Invalid config file contents [location's close bracket error]"));
    return (new_location);
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

void Server::setClientMaxBodySize(long long client_max_body_size)
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

void Server::setIndex(std::string &index)
{
    index_ = index;
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

long long Server::getClientMaxBodySize(void)
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

std::string Server::getIndex(void)
{
    return (index_);
}

Location Server::getLocationBlock(int idx)
{
    return (location_block_[idx]);
}

std::vector<Location> Server::getLocation(void)
{
    return (location_block_);
}

// util
// int Server::findLocationBlock(std::string url)   // 도메인 뒤에 url로 넘겨주도록
// {
//     for(size_t i = 0; i < location_block_.size(); i++)
//     {
//         if (location_block_[i].getUrlPostfix() == url)
//         {
//             return (i);
//         }
//     }
//     return (-1);
// }

void Server::setLocation(Location &location)
{
    location_block_.push_back(location);
}

void Server::pushBackLocationBlock(Location &location)
{
    location_block_.push_back(location);
}

// void Server::pushFrontLocationBlock(Location &location)
// {
//     location_block_.push_back(location);
// }

void Server::fixLocation(int idx, Location &location)
{
    location_block_[idx] = location;
}

void Server::createDefaultLocation()
{
    location_block_.push_back(*this);
}
