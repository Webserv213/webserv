#ifndef HTTP_HPP
# define HTTP_HPP
# include "Server.hpp"
# include "Location.hpp"
# include <vector>
# include "Utils.hpp"
# include <fstream>

# define DEFAULT_CONF_FILE_PATH "./conf/1webserv.conf"
# define BUFFER_SIZE 100000
# define SAME 0
# define HTTPBLOCK_CLOSE_OK 0

class Http
{
private:
    std::vector<Server> server_block_;
    int                 close_http_bracket;

public:
    Http();
    ~Http();

    void setWebserv(int argc, char **argv);
    void setHttpBlock(std::istringstream& stream_file_contents);
    Server setServerBlock(std::istringstream& stream_file_contents);
    void pushBackServerBlock(Server &server);
    std::vector<Server>& getServer();
};

#endif
