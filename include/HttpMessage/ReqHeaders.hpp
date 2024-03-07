#ifndef REQHEADERS_HPP
# define REQHEADERS_HPP
# include <iostream>

class ReqHeaders
{
private:
    int             content_length_;
    std::string     content_type_;
    std::string     accept_;
    std::string     accept_encoding_;
    std::string     accept_language_;
    std::string     connection_;
    std::string     host_;
    unsigned short  listen_port_;
    std::string     url_postfix_;
    std::string     upgrade_insecure_requests_;
    std::string     user_agent_;

public:
    ReqHeaders();
    ~ReqHeaders();

    void    setContentLength(int& content_length);
    void    setContentType(std::string& content_type);
    void    setAccept(std::string& accept);
    void    setAcceptEncoding(std::string& accept_encoding);
    void    setAcceptLanguage(std::string& accept_language);
    void    setConnection(std::string& connection);
    void    setFullPath(std::string& host);
    void    setUpgradeInsecureRequests(std::string& upgrade_insecure_requests);
    void    setUserAgent(std::string& user_agent);

    int             getContentLength();
    std::string     getContentType();
    std::string     getAccept();
    std::string     getAcceptEncoding();
    std::string     getAcceptLanguage();
    std::string     getConnection();
    std::string     getHost();
    unsigned short  getListenPort();
    std::string     getUrlPostfix();
    std::string     getUpgradeInsecureRequests();
    std::string     getUserAgent();
};

#endif