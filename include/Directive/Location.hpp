#ifndef LOCATION_HPP
# define LOCATION_HPP
# include "Server.hpp"

class Location : public Server
{
private:    
    std::string                 operation_;
    std::vector<std::string>    path_postfix_;

public:
    Location();
    ~Location();
};

#endif