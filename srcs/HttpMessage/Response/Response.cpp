#include "Response.hpp"

Response::Response(void)
{
    body_ = "";
}

Response::~Response(void)
{
    
}

ResponseLine Response::getResponseLine(void)
{
    return (status_line_);
}

ResHeaders  Response::getResHeaders(void)
{
    return (headers_);
}

std::string Response::getBody(void)
{
    return(body_);
}

void    Response::setBody(std::string body)
{
    body_ += body;
}