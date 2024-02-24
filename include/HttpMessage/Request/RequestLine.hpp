#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP
# include <iostream>

struct RequestLine
{
    std::string method;
    std::string request_target;
    std::string version;
};

#endif