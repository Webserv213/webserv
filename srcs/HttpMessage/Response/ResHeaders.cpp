#include "ResHeaders.hpp"

ResHeaders::ResHeaders(void)
{
    std::string server_ = "default_server";
    std::string date_ = "";
    std::string content_type_= "none";
    std::string last_modified_= "none";
    std::string transfer_encoding_= "none";
    std::string connection_ = "none";
    std::string content_encoding_ = "none";
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

