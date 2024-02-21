#include "Server.hpp"

Server::Server() : server_name_("webserv.com"), \
                    root_("./www"), \
                    autoindex_(true), \
                    client_max_body_size_(10000)
{
    // Listen listen_;
    // std::map<int, std::string> error_page_;
    // std::vector<Location> location_block_;
}

Server& Server::operator=(const Server& other)
{
	if (this != &other)
        ;
	return (*this);
}

Server::Server(const Server &other)
{
	*this = other;
}

Server::~Server()
{
}