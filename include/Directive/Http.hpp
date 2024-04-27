#ifndef HTTP_HPP
# define HTTP_HPP

# include "Server.hpp"
# include "Location.hpp"
# include "Utils.hpp"

# define DEFAULT_CONF_FILE_PATH "./conf/default.conf"
# define BUFFER_SIZE 65536
# define HTTPBLOCK_CLOSE_OK 0

class Http
{
private:
    std::vector<Server> server_block_;
    int                 close_http_bracket;

public:
    Http();
    ~Http();

    //getter
    std::vector<Server>& getServer();
    
    //setter
    void setWebserv(int argc, char **argv);
    void setHttpBlock(std::istringstream& stream_file_contents);
    Server setServerBlock(std::istringstream& stream_file_contents);

    //utils
    void pushBackServerBlock(Server &server);
};

#endif