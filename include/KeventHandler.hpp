#ifndef KEVENT_HANDLER_HPP
# define KEVENT_HANDLER_HPP

# include <iostream>
# include <map>
# include <vector>
# include <sys/event.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <sstream>
# include "EventRecorder.hpp"
# include "Http.hpp"

class KeventHandler
{
private:
    Http                                http_;
    std::vector<uintptr_t>              server_sockets_;
    std::map<int, EventRecorder>        fd_manager_;
    std::map< int, std::vector<char> >  data_;
    std::vector<struct kevent>          change_list_;
    // event_list의 크기 생각해보기
    struct kevent event_list_[100];

    void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
    
public:
    KeventHandler(Http &http);
    ~KeventHandler();
    void openListenSocket();
    void tmp();
};

# endif