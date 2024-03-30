#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include "RequestLine.hpp"
# include "ReqHeaders.hpp"
# include "Server.hpp"
# include <fcntl.h>
# include <unistd.h>

class Request
{
private:
    RequestLine start_line_;
    ReqHeaders  headers_;
    std::string body_;

public:
    RequestLine& getRequestLine();
    ReqHeaders& getHeaders();
    std::string& getBody();
    void methodGetHandler(Server& server);
    void addBody(std::string& body);
    void setBody(std::string& body);
    Request();
    ~Request();
};

#endif