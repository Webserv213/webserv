#include "Location.hpp"
#include "Server.hpp"

Location::Location(void)
{
    autoindex_ = false;
    client_max_body_size_ = -1;
    error_page_["404"] = "./var/www/error";
    cgi_path_ = "";
}

Location::Location(Server parent_server)
{
    root_ = parent_server.getRoot();
    autoindex_ = parent_server.getAutoIndex();
    client_max_body_size_ = parent_server.getClientMaxBodySize();
    error_page_["404"] = parent_server.getErrorPage("404");
    index_ = parent_server.getIndex();
    cgi_path_ = "";
}

Location::~Location(void)
{

}

// getter
std::vector<std::string>& Location::getUrlPostfix()
{
    return (url_postfix_);
}

std::vector<std::string>& Location::getAccessMethod()
{
    return (access_method_);
}

std::string& Location::getRoot()
{
    return (root_);
}

std::string& Location::getIndex()
{
    return (index_);
}

bool Location::getAutoIndex()
{
    return (autoindex_);
}

long long Location::getClientMaxBodySize()
{
    return (client_max_body_size_);
}

std::string& Location::getCgiPath()
{
    return (cgi_path_);
}

// setter
void Location::setRoot(std::string &root)
{
    root_ = root;
}

void Location::setIndex(std::string &index)
{
    index_ = index;
}

void Location::setAutoIndex(bool flag)
{
    autoindex_ = flag;
}

void Location::setClientMaxBodySize(long long client_max_body_size)
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

void Location::setAccessMethod(std::string  access_method)
{
    access_method_.push_back(access_method);
}

void Location::setErrorPage(std::string error_code, std::string &path)
{
    error_page_[error_code] = path;
}

void Location::setCgiPath(std::string path)
{
    cgi_path_ = path;
}
