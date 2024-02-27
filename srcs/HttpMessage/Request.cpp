#include "Request.hpp"

Request::Request(void)
{
    body_ = "";
}

Request::~Request(void)
{
    
}

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

void Request::setBody(std::string& body)
{
    body_ += body;
}
