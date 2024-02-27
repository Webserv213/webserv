#include "ReqHeaders.hpp"

ReqHeaders::ReqHeaders(void)
{
    // int         content_length_ = 0;
    std::string content_type_ = "none";
    std::string accept_ = "none";
    std::string accept_encoding_ = "none";
    std::string accept_language_ = "none";
    std::string connection_ = "none";
    std::string host_ = "webserv.com";
    std::string upgrade_insecure_requests_ = "none";
    std::string user_agent_ = "none";
}

ReqHeaders::~ReqHeaders(void)
{
    
}

// setter
void    ReqHeaders::setContentLength(int& content_length)
{
    this->content_length_ = content_length;
}

void    ReqHeaders::setContentType(std::string& content_type)
{
    this->content_type_ = content_type;
}

void    ReqHeaders::setAccept(std::string& accept)
{
    this->accept_ = accept;
}

void    ReqHeaders::setAcceptEncoding(std::string& accept_encoding)
{
    this->accept_encoding_ = accept_encoding;
}

void    ReqHeaders::setAcceptLanguage(std::string& accept_language)
{
    this->accept_language_ = accept_language;
}

void    ReqHeaders::setConnection(std::string& connection)
{
    this->connection_ = connection;
}

void    ReqHeaders::setHost(std::string& host)
{
    this->host_ = host;
}

void    ReqHeaders::setUpgradeInsecureRequests(std::string& upgrade_insecure_requests)
{
    this->upgrade_insecure_requests_ = upgrade_insecure_requests;
}

void    ReqHeaders::setUserAgent(std::string& user_agent)
{
    this->user_agent_ = user_agent;
}

//getter

