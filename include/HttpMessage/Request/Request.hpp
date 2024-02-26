#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include "RequestLine.hpp"
# include "ReqHeaders.hpp"

class Request
{
private:
    RequestLine start_line_;
    ReqHeaders  headers_;
    std::string body_;

public:
    RequestLine getRequestLine();
    ReqHeaders getHeaders();
    std::string getBody();
    void setBody(std::string body);
    Request();
    ~Request();
};

#endif