#include "Response.hpp"

Response::Response(void)
{
}

Response::~Response(void)
{
    
}

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

void    Response::addBody(const std::string& body)
{
    body_.append(body);
}

void    Response::setBody(const std::string& body)
{
    body_ = body;
}