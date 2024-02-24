#ifndef HEADERS_HPP
# define HEADERS_HPP
# include <iostream>

class Headers
{
private:
    std::string server_;
    std::string date_;
    std::string content_type_;
    std::string last_modified_;
    std::string transfer_encoding_;
    std::string connection_;
    std::string content_encoding_;

public:
    Headers();
    ~Headers();
};

#endif