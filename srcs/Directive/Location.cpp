#include "Location.hpp"
#include "Server.hpp"

Location::Location(void)
{
    // root_ = "./var/www/default";
    autoindex_ = true;
    client_max_body_size_ = 10000;
    error_page_["404"] = "./var/www/error";
}

Location::Location(Server parent_server)
{
    root_ = parent_server.getRoot();
    autoindex_ = parent_server.getAutoIndex();
    client_max_body_size_ = parent_server.getClientMaxBodySize();
    error_page_["404"] = parent_server.getErrorPage("404");
}

Location::~Location(void)
{

}

std::vector<std::string> Location::getUrlPostfix()
{
    return (url_postfix_);
}

std::string Location::getRoot()
{
    return (root_);
}

bool Location::getAutoIndex()
{
    return (autoindex_);
}

unsigned long long Location::getClientMaxBodySize()
{
    return (client_max_body_size_);
}


void Location::setRoot(std::string &root)
{
    root_ = root;
}

void Location::setAutoIndex(bool flag)
{
    autoindex_ = flag;
}

void Location::setClientMaxBodySize(unsigned long long client_max_body_size)
{
    client_max_body_size_ = client_max_body_size;
}

void Location::setUrlPostfix(std::string  url_postfix)
{
    std::string buf;

    std::istringstream streamLine(url_postfix);
    while (std::getline(streamLine, buf, '/'))
    {
        if (buf != "")
            url_postfix_.push_back(buf);
    }
}

void Location::setErrorPage(std::string error_code, std::string &path)
{
    error_page_[error_code] = path;
}
