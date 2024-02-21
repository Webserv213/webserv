#ifndef SERVER_HPP
# define SERVER_HPP

#include "Location.hpp"
#include "Listen.hpp"

#include <string>
#include <map>
#include <vector>

class Server {
    public :
        Server();
        Listen listen_;
        std::string server_name_;
        std::string root_;
        std::map<int, std::string> error_page_;
        bool autoindex_;
        // std::vector<Location> location_block_;
        unsigned long long client_max_body_size_;

        virtual ~Server();
    private :
        Server(const Server &other);
        Server& operator=(const Server& other);
};

#endif