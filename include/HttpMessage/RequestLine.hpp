#ifndef REQUESTLINE_HPP
# define REQUESTLINE_HPP

# include <vector>
# include <sstream>

class RequestLine
{
private :
    std::string                 method_;
    std::vector<std::string>    request_target_;
    std::string                 version_;

public :
    RequestLine();
    ~RequestLine();

    //getter
    std::string&                getMethod(void);
    std::vector<std::string>&    getRequestTarget(void);
    std::string&                getVersion(void);

    //setter
    void    setMethod(std::string& method);
    void    setRequestTarget(std::string& request_target);
    void    setVersion(std::string& request_target);

};

#endif