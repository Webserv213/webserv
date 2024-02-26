#include "ResponseLine.hpp"

ResponseLine::ResponseLine(void)
{
    std::string method_ = "GET";
    std::string request_target_ = "";
    std::string version_ = "";
}

ResponseLine::~ResponseLine(void)
{
    
}

void    ResponseLine::setVersion(std::string version)
{
    version_ = version;
}

void    ResponseLine::setStatusCode(std::string status_code)
{
    status_code_ = status_code;
}

void    ResponseLine::setStatusText(std::string status_text)
{
    status_text_ = status_text;
}
