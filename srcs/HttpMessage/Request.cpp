#include "Request.hpp"

Request::Request(void)
{
    body_ = "";
}

Request::~Request(void)
{
    
}

//getter
RequestLine& Request::getRequestLine()
{
    return (start_line_);
}

ReqHeaders& Request::getHeaders()
{
    return (headers_);
}

std::string& Request::getBody(void)
{
    return (body_);
}

//setter
void Request::setBody(const std::string& body)
{
    body_ = body;
}

//utils
void Request::addBody(const std::string& body)
{
    body_.append(body);
}


