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

    void    setContentLength(int content_length);
    void    setContentType(std::string content_type);
    void    setAccept(std::string accept);
    void    setAcceptEncoding(std::string accept_encoding);
    void    setAcceptLanguage(std::string accept_language);
    void    setConnection(std::string connection);
    void    setHost(std::string host);
    void    setUpgradeInsecureRequests(std::string upgrade_insecure_requests);
    void    setUserAgent(std::string user_agent);
};

#endif