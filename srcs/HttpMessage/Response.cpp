#include "Response.hpp"

Response::Response(void)
{
    body_ = "";
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

void    Response::setBody(std::string& body)
{
    body_ += body;
}