#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ResponseLine.hpp"
# include "ResHeaders.hpp"

class Response
{
private:
    ResponseLine    status_line_;
    ResHeaders      headers_;
    std::string     body_;

public:
    Response();
    ~Response();

    //getter
    ResponseLine&   getStatusLine(void);
    ResHeaders&     getHeaders(void);
    std::string&    getBody(void);

    //setter
    void            setBody(const std::string& body);

    //utils
    void            addBody(const std::string& body);
};

#endif