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

std::vector<char>& Response::getBody(void)
{
    return(body_);
}

void    Response::setBody(std::string& body)
{
    for (size_t i = 0; i < body.size(); i++)
        body_.push_back(body[i]);
}