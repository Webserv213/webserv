#ifndef LOCATION_HPP
# define LOCATION_HPP
# include <iostream>

class Location
{
private:    
    std::string operation_;
    std::string path_postfix_;

public:
    Location();
    ~Location();

    // getter
    std::string getPathPostfix();
};

#endif