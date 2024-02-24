#ifndef HEADERS_HPP
# define HEADERS_HPP
# include <iostream>

class Headers
{
private:
    int         content_length_;
    std::string content_type_;
    std::string accept_;
    std::string accept_encoding_;
    std::string accept_language_;
    std::string connection_;
    std::string host_;
    std::string upgrade_insecure_requests_;
    std::string user_agent_;

public:
    Headers();
    ~Headers();
};

#endif