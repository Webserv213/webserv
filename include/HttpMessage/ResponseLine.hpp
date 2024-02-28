#ifndef RESPONSELINE_HPP
# define RESPONSELINE_HPP
# include <iostream>

struct ResponseLine
{
public :
    ResponseLine();
    ~ResponseLine();
    void            setVersion(std::string version);
    void            setStatusCode(std::string status_code);
    void            setStatusText(std::string status_text);
    std::string&    getVersion(void);
    std::string&    getStatusCode(void);
    std::string&    getStatusText(void);
private :
    std::string version_;
    std::string status_code_;
    std::string status_text_;
};

#endif