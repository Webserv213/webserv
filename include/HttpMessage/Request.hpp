#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Server.hpp"
# include "ReqHeaders.hpp"
# include "RequestLine.hpp"

class Request
{
private:
    RequestLine start_line_;
    ReqHeaders  headers_;
    std::string body_;

public:
    Request();
    ~Request();
    
    //getter
    RequestLine& getRequestLine();
    ReqHeaders& getHeaders();
    std::string& getBody();

    //setter
    void setBody(const std::string& body);

    //utils
    void addBody(const std::string& body);
};

#endif