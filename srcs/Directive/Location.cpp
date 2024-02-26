#include "Location.hpp"
#include "Server.hpp"

Location::Location(void)
{
    operation_ = "/";
    path_postfix_ = "sooha";
}

Location::~Location(void)
{
    
}

std::string Location::getPathPostfix(void)
{
    return (path_postfix_);
}