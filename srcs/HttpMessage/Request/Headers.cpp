#include "Headers.hpp"

Headers::Headers(void)
{

}

Headers::~Headers(void)
{
    
}

// setter
void    Headers::setContentLength(int content_length)
{
    this->content_length_ = content_length;
}

void    Headers::setContentType(std::string content_type)
{
    this->content_type_ = content_type;
}

void    Headers::setAccept(std::string accept)
{
    this->accept_ = accept;
}

void    Headers::setAcceptEncoding(std::string accept_encoding)
{
    this->accept_encoding_ = accept_encoding;
}

void    Headers::setAcceptLanguage(std::string accept_language)
{
    this->accept_language_ = accept_language;
}

void    Headers::setConnection(std::string connection)
{
    this->connection_ = connection;
}

void    Headers::setHost(std::string host)
{
    this->host_ = host;
}

void    Headers::setUpgradeInsecureRequests(std::string upgrade_insecure_requests)
{
    this->upgrade_insecure_requests_ = upgrade_insecure_requests;
}

void    Headers::setUserAgent(std::string user_agent)
{
    this->user_agent_ = user_agent;
}

//getter

