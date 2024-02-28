#include "RequestLine.hpp"

RequestLine::RequestLine(void)
{
    std::string method_ = "GET";
    std::string request_target_ = "";
    std::string version_ = "";
}

RequestLine::~RequestLine(void)
{
    
}

void    RequestLine::setMethod(std::string& method)
{
    method_ = method;
}

void    RequestLine::setRequestTarget(std::string& request_target)
{
    request_target_ = request_target;
}

void    RequestLine::setVersion(std::string& version)
{
    version_ = version;
}

std::string&    RequestLine::getMethod(void)
{
    return (method_);
}

std::string&    RequestLine::getRequestTarget(void)
{
    return (request_target_);
}

std::string&    RequestLine::getVersion(void)
{
    return (version_);
}
