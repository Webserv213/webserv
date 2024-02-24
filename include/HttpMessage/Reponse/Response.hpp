#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <iostream>
# include "ResponseLine.hpp"
# include "Headers.hpp"
# include "Body.hpp"

class Response
{
private:
    ResponseLine    status_line_;
    Headers         *headers_;
    Body            body_;

public:
    Response();
    ~Response();
};

#endif