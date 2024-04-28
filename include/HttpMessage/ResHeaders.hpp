#ifndef RESHEADERS_HPP
# define RESHEADERS_HPP

# include <string>

class ResHeaders
{
private:
    std::string server_;
    std::string date_;
    std::string content_type_;
    std::string last_modified_;
    std::string transfer_encoding_;
    std::string connection_;
    std::string content_encoding_;
    std::string content_length_;

public:
    ResHeaders();
    ~ResHeaders();

    //getter
    std::string&    getServer(void);
    std::string&    getDate(void);
    std::string&    getContentType(void);
    std::string&    getLastModified(void);
    std::string&    getTransferEncoding(void);
    std::string&    getConnection(void);
    std::string&    getContentEncoding(void);
    std::string&    getContentLength(void);

    //setter
    void    setServer(std::string server);
    void    setDate(std::string date);
    void    setContentType(std::string content_type);
    void    setLastModified(std::string last_modified);
    void    setTransferEncoding(std::string transfer_encoding);
    void    setConnection(std::string connection);
    void    setContentEncoding(std::string content_encoding);
    void    setContentLength(std::string content_length);
};

#endif