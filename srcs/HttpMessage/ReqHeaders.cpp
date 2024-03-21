#include "ReqHeaders.hpp"

ReqHeaders::ReqHeaders(void)
{
    content_length_ = 0;
    content_type_ = "";
    accept_ = "";
    accept_encoding_ = "";
    accept_language_ = "";
    connection_ = "";
    host_ = "";
    listen_port_ = 0;
    // url_postfix_ = "";
    upgrade_insecure_requests_ = "";
    user_agent_ = "";
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

// sooha.webserv.com:8082  or  sooha.webserv.com:8082/location -> 이거 슬래시 안들어옴
void    ReqHeaders::setFullPath(std::string& host)
{
    std::string listen_port_str;

    for (size_t i = 0; i < host.size(); i++)
    {
        if (host[i] == ':' || host[i] == '/')
        {
            while (i < host.size())
            {
                if (host[i] == ':')
                {
                    i++;
                    while (i < host.size())
                    {
                        if (host[i] == '/')
                        {
                            listen_port_ = std::atoi(listen_port_str.c_str());

                            std::string str = "";
                            i++;
                            while (i < host.size())
                            {
                                str += host[i];
                                i++;
                            }
                            // url_postfix_.push_back(str);
                            return ;
                        }
                        else
                        {
                            listen_port_str += host[i];
                        }
                        i++;
                    }
                    listen_port_ = std::atoi(listen_port_str.c_str());
                    return ;
                }
                else if (host[i] == '/')
                {
                    while (i < host.size())
                    {
                        // url_postfix_ += host[i];
                        ;
                        i++;
                    }   
                    return ; 
                }
                i++;
            }
        }
        else
        {
            host_ += host[i];
        }
    }
}

void    ReqHeaders::setUpgradeInsecureRequests(std::string& upgrade_insecure_requests)
{
    this->upgrade_insecure_requests_ = upgrade_insecure_requests;
}

void    ReqHeaders::setUserAgent(std::string& user_agent)
{
    this->user_agent_ = user_agent;
}

void    ReqHeaders::setTransferEncoding(std::string& transfer_encoding)
{
    this->transfer_encoding_ = transfer_encoding;
}

//getter

int             ReqHeaders::getContentLength()
{
    return (content_length_);
}

std::string     ReqHeaders::getContentType()
{
    return (content_type_);
}

std::string     ReqHeaders::getAccept()
{
    return (accept_);
}

std::string     ReqHeaders::getAcceptEncoding()
{
    return (accept_encoding_);
}

std::string     ReqHeaders::getAcceptLanguage()
{
    return (accept_language_);
}

std::string     ReqHeaders::getConnection()
{
    return (connection_);
}

std::string     ReqHeaders::getHost()
{
    return (host_);
}

unsigned short  ReqHeaders::getListenPort()
{
    return (listen_port_);
}

// std::string     ReqHeaders::getUrlPostfix()
// {
//     return (url_postfix_);
// }

std::string     ReqHeaders::getUpgradeInsecureRequests()
{
    return (upgrade_insecure_requests_);
}

std::string     ReqHeaders::getUserAgent()
{
    return (user_agent_);
}

std::string     ReqHeaders::getTransferEncoding()
{
    return (transfer_encoding_);
}

