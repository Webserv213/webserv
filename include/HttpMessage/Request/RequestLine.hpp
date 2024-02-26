#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP
# include <iostream>

struct RequestLine
{
public :
    void    setMethod(std::string method);
    void    setRequestTarget(std::string request_target);
    void    setVersion(std::string request_target);

private :
    std::string method_;
    std::string request_target_;
    std::string version_;
};

#endif