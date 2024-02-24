#include "Http.hpp"

Http::Http(void)
{

}

Http::~Http(void)
{
    
}

void Http::setServer(Server &server)
{
    server_block_.push_back(server);
}
