#ifndef LISTEN_HPP
# define LISTEN_HPP

#include <vector>
#include <string>

class Listen {
  public :
    Listen();
    std::string ip_;
    unsigned short port_;
    
    virtual ~Listen();
  private :
    Listen(const Listen &other);
    Listen& operator=(const Listen& other);
};

#endif