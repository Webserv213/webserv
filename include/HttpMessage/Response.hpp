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
    std::vector<char>   body_;

public:
    ResponseLine&       getStatusLine(void);
    ResHeaders&         getHeaders(void);
    std::vector<char>&  getBody(void);
    void                setBody(std::string& body);
    Response();
    ~Response();
};

#endif
