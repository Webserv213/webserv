#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>
# include <map>
# include <vector>

class Server;

class Location
{
private:
    std::string                         root_;
    std::vector<std::string>            url_postfix_;
    std::string                         index_;
    bool                                autoindex_;
    unsigned long long                  client_max_body_size_;
    std::map<std::string, std::string>  error_page_;
    std::vector<std::string>            access_method_;
    std::string                         cgi_path_;

public:
    Location();
    Location(Server parent_server);
    ~Location();

    // getter
    std::vector<std::string>&   getUrlPostfix();
    std::vector<std::string>&   getAccessMethod();
    std::string&                getRoot();
    std::string&                getIndex();
    std::string&                getCgiPath();
    long long                   getClientMaxBodySize();
    bool                        getAutoIndex();

    // setter
    void setRoot(std::string &root);
    void setIndex(std::string &index);
    void setAutoIndex(bool flag);
    void setClientMaxBodySize(long long client_max_body_size);
    void setUrlPostfix(std::string url_postfix);
    void setAccessMethod(std::string access_method);
    void setErrorPage(std::string error_code, std::string &path);
    void setCgiPath(std::string path);
};

#endif
