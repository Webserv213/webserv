#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <map>
# include "Server.hpp"

class Server;

class Location
{
private:
    std::string                         root_;
    std::vector<std::string>            url_postfix_;
    bool                                autoindex_;
    unsigned long long                  client_max_body_size_;
    std::map<std::string, std::string>  error_page_;
    std::vector<std::string>            access_method_;

public:
    Location();
    Location(Server parent_server);
    ~Location();

    // getter
    std::vector<std::string>    getUrlPostfix();
    std::string                 getRoot();
    bool                        getAutoIndex();
    unsigned long long          getClientMaxBodySize();

    void setRoot(std::string &root);
    void setAutoIndex(bool flag);
    void setClientMaxBodySize(unsigned long long client_max_body_size);
    void setUrlPostfix(std::string  url_postfix);
    void setErrorPage(std::string error_code, std::string &path);

};

#endif
