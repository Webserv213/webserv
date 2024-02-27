#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP
# include <iostream>

class RequestLine
{
public :
    RequestLine();
    ~RequestLine();
    void            setMethod(std::string method);
    void            setRequestTarget(std::string request_target);
    void            setVersion(std::string request_target);
    std::string&    getMethod(void);
    std::string&    getRequestTarget(void);
    std::string&    getVersion(void);

private :
    std::string method_;
    std::string request_target_;
    std::string version_;
};

#endif