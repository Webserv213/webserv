#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <vector>
#include <string>
#include <map>

class Location {
    public :
        Location();
        std::string operation_;
        std::string root_;
        std::vector<std::string> path_postfix_;
        bool autoindex_ ;
        std::map<int, std::string> error_page_;
        // std::vector<Location> location_block_;
        unsigned long long client_max_body_size_;
    private :
        Location(const Location &other);
        Location& operator=(const Location& other);
        virtual ~Location();
};

#endif