#ifndef RESPONSELINE_HPP
# define RESPONSELINE_HPP
# include <iostream>

struct ResponseLine
{
    std::string version_;
    std::string status_code_;
    std::string status_text_;
};

#endif