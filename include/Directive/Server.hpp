#ifndef SERVER_HPP
# define SERVER_HPP

# include "Utils.hpp"

# include <sstream>
# include <string>
# include <map>
# include <vector>

class Location;

class Server
{
private:
    unsigned short                      listen_port_;
    std::string                         server_name_;
    std::string                         root_;
    std::string                         index_;
    bool                                autoindex_;
    long long                           client_max_body_size_;
    std::map<std::string, std::string>  error_page_;
    // [0] : default location
    std::vector<Location>               location_block_;

public:
    Server();
    ~Server();

    //getter
    std::string &getRoot();
    bool getAutoIndex();
    long long getClientMaxBodySize();
    std::string &getErrorPage(std::string error_code);
    unsigned short getListenPort();
    std::string &getServerName();
    Location &getLocationBlock(int idx);
    std::vector<Location> &getLocation();
    std::string &getIndex();

    //setter
    Location setLocationBlock(std::istringstream& stream_file_contents);
    void setRoot(std::string &root);
    void setAutoIndex(bool flag);
    void setClientMaxBodySize(long long client_max_body_size);
    void setErrorPage(std::string error_code, std::string &path);
    void setListenPort(std::string &listen_port);
    void setServerName(std::string &server_name);
    void setLocation(Location &location);
    void setIndex(std::string &index);

    //utils
    void pushBackLocationBlock(Location &location);
    void fixLocation(int idx, Location &location);
    void createDefaultLocation();
};

#endif
