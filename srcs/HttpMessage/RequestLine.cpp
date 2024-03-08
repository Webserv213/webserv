#include "RequestLine.hpp"

RequestLine::RequestLine(void)
{
    method_ = "GET";
    version_ = "";
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
    std::string buf;
    std::istringstream streamLine(request_target);

    while (std::getline(streamLine, buf, '/'))
        request_target_.push_back(buf);
}

void    RequestLine::setVersion(std::string& version)
{
    version_ = version;
}

std::string&    RequestLine::getMethod(void)
{
    return (method_);
}

std::vector<std::string>    RequestLine::getRequestTarget(void)
{
    return (request_target_);
}

std::string&    RequestLine::getVersion(void)
{
    return (version_);
}
