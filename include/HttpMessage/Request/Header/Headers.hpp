#ifndef HEADERS_HPP
# define HEADERS_HPP
# include <iostream>

class Headers
{
protected:
    std::string accept;
    std::string accept_encoding;
    std::string accept_language;
    std::string connection;
    std::string host;
    std::string upgrade_insecure_requests;
    std::string user_agent;

public:
    Headers();
    ~Headers();
};

#endif