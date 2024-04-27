#include "Response.hpp"

Response::Response(void)
{
}

Response::~Response(void)
{
    
}

//getter
ResponseLine& Response::getStatusLine(void)
{
    return (status_line_);
}

ResHeaders&  Response::getHeaders(void)
{
    return (headers_);
}

std::string& Response::getBody(void)
{
    return(body_);
}

//setter
void    Response::setBody(const std::string& body)
{
    body_ = body;
}

//utils
void    Response::addBody(const std::string& body)
{
    body_.append(body);
}