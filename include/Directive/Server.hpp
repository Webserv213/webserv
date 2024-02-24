#ifndef SERVER_HPP
# define SERVER_HPP
# include "Location.hpp"
# include <iostream>
# include <map>

class Server
{
protected:
    std::string                 root_;
    bool                        autoindex_;
    unsigned long long          client_max_body_size_;
    std::map<int, std::string>  error_page_;

private:
    std::map<std::string, unsigned short> listen_;
    std::string server_name_;
    std::vector<Location> location_block_;

public:
    Server();
    ~Server();

    void setLocation(Location &location);
};

#endif