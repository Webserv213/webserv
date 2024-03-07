#include "ResHeaders.hpp"

ResHeaders::ResHeaders(void)
{
    server_ = "CLK-Webserv";
    date_ = "";
    content_type_= "";
    last_modified_= "";
    transfer_encoding_= "";
    connection_ = "";
    content_encoding_ = "";
}

ResHeaders::~ResHeaders(void)
{
    
}

// setter
void    ResHeaders::setServer(std::string server)
{
    server_ = server;
}

void    ResHeaders::setDate(std::string date)
{
    date_ = date;
}

void    ResHeaders::setContentType(std::string content_type)
{
    content_type_ = content_type;
}

void    ResHeaders::setLastModified(std::string last_modified)
{
    last_modified_ = last_modified;
}

void    ResHeaders::setTransferEncoding(std::string transfer_encoding)
{
    transfer_encoding_ = transfer_encoding;
}

void    ResHeaders::setConnection(std::string connection)
{
    connection_ = connection;
}

void    ResHeaders::setContentEncoding(std::string content_encoding)
{
    content_encoding_ = content_encoding;
}

void    ResHeaders::setContentLength(std::string content_length)
{
    content_length_ = content_length;
}

void    ResHeaders::setKeepAlive(std::string keep_alive)
{
    keep_alive_ = keep_alive;
}

std::string&    ResHeaders::getServer(void)
{
    return (server_);
}

std::string&    ResHeaders::getDate(void)
{
    return (date_);
}

std::string&    ResHeaders::getContentType(void)
{
    return (content_type_);
}

std::string&    ResHeaders::getLastModified(void)
{
    return (last_modified_);
}

std::string&    ResHeaders::getTransferEncoding(void)
{
    return (transfer_encoding_);
}

std::string&    ResHeaders::getConnection(void)
{
    return (connection_);
}

std::string&    ResHeaders::getContentEncoding(void)
{
    return (content_encoding_);
}

std::string&    ResHeaders::getContentLength(void)
{
    return (content_length_);
}

std::string&    ResHeaders::getKeepAlive(void)
{
    return (keep_alive_);
}