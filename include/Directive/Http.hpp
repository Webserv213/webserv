#ifndef HTTP_HPP
# define HTTP_HPP
#include "Server.hpp"
# include <vector>

class Http
{
private:
    std::vector<Server> server_block_;

public:
    Http();
    ~Http();

    void setServer(Server &server);
};

#endif