#include "Server.hpp"

Server::Server(void)
{
    
}

Server::~Server(void)
{
    
}

void Server::setLocation(Location &location)
{
    location_block_.push_back(location);
}
