#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <iostream>
# include "ResponseLine.hpp"
# include "ResHeaders.hpp"

class Response
{
private:
    ResponseLine    status_line_;
    ResHeaders      headers_;
    std::string     body_;

public:
    ResponseLine&   getResponseLine(void);
    ResHeaders&     getResHeaders(void);
    std::string&    getBody(void);
    void            setBody(std::string& body);
    Response();
    ~Response();
};

#endif
