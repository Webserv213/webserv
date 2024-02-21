#ifndef HTTP_HPP
# define HTTP_HPP

#include "Server.hpp"
#include <vector>

class Http {
  public :
    Http();
    std::vector<Server> server_block_;
    bool autoindex_;
    std::string root_;
    std::map<int, std::string> error_page_;
    unsigned long long client_max_body_size_;
  private :
    Http(const Http &other);
    Http& operator=(const Http& other);
    virtual ~Http();
};

#endif