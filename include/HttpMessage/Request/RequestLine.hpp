#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP
# include <iostream>

struct RequestLine
{
    std::string method_;
    std::string request_target_;
    std::string version_;
};

#endif