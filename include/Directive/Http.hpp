#ifndef HTTP_HPP
# define HTTP_HPP
# include "Server.hpp"
# include <vector>

# define DEFAULT_CONFIG_PATH "./conf/webserv.conf"
# define BUFFER_SIZE 100000

class Http
{
private:
    std::vector<Server> server_block_;

public:
    Http();
    ~Http();

    void setServer(Server &server);
    std::vector<Server>& getServer();
    void setWebserv(int argc, char **argv);
    void setServerValue(std::string &get_str, Server &new_server);
};

#endif
