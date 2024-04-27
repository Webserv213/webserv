#ifndef RESPONSELINE_HPP
# define RESPONSELINE_HPP

# include <string>

struct ResponseLine
{
private :
    std::string version_;
    std::string status_code_;
    std::string status_text_;
public :
    ResponseLine();
    ~ResponseLine();
    
    //getter
    std::string&    getVersion(void);
    std::string&    getStatusCode(void);
    std::string&    getStatusText(void);

    //setter
    void    setVersion(std::string version);
    void    setStatusCode(std::string status_code);
    void    setStatusText(std::string status_text);

};

#endif