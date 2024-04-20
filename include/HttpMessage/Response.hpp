#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include <iostream>
# include "ResponseLine.hpp"
# include "ResHeaders.hpp"
# include <vector>

class Response
{
private:
    ResponseLine        status_line_;
    ResHeaders          headers_;
    std::string         body_;

public:
    ResponseLine&       getStatusLine(void);
    ResHeaders&         getHeaders(void);
    std::string&        getBody(void);
    void                addBody(std::string& body);
    Response();
    ~Response();
};

#endif
