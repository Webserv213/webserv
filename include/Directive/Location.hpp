#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <map>

class Location
{
private:    
    std::string                 url_postfix_;
    std::string                 root_;
    bool                        autoindex_;
    unsigned long long          client_max_body_size_;
    std::map<std::string, std::string>  error_page_;

public:
    Location();
    ~Location();

    // getter
    std::string         getUrlPostfix();
    std::string         getRoot();
    bool                getAutoIndex();
    unsigned long long  getClientMaxBodySize();

    void setRoot(std::string &root);
    void setAutoIndex(bool flag);
    void setClientMaxBodySize(unsigned long long client_max_body_size);
    void setUrlPostfix(std::string  url_postfix);
    void setErrorPage(std::string error_code, std::string &path);

};

#endif