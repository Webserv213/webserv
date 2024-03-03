#ifndef SERVER_HPP
# define SERVER_HPP
# include "Location.hpp"
# include <iostream>
# include <map>
# include <string>
# include <vector>

class Server
{
private:
    std::string                 root_;
    bool                        autoindex_;
    unsigned long long          client_max_body_size_;
    std::map<int, std::string>  error_page_;
    std::map<std::string, unsigned short>   listen_;
    std::string                             server_name_;
    std::vector<Location>                   location_block_;

public:
    Server();
    ~Server();

    //getter
    std::string getRoot();
    bool getAutoIndex();
    unsigned long long getClientMaxBodySize();
    std::string getErrorPage(int idx);
    unsigned short getListen(std::string ip);
    std::string getServerName();
    Location getLocationBlock(int idx);

    //setter
    void setRoot(std::string &root);
    void setAutoIndex(bool flag);
    void setClientMaxBodySize(unsigned long long client_max_body_size);
    void setErrorPage(int error_num, std::string &path);


    void setListen(std::string &ip, unsigned short port);
    void setServerName(std::string &server_name);
    void setLocation(Location &location);

    // util
    int findLocationBlock(std::string url);
};

#endif
