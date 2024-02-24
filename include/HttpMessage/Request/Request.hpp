#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include "RequestLine.hpp"
# include "Headers.hpp"
# include "Body.hpp"

class Request
{
private:
    RequestLine start_line_;
    Headers     headers_;
    Body        body_;

public:
    Request();
    ~Request();
};

#endif