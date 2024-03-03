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
# include <ctime>

# include "EventRecorder.hpp"
# include "Http.hpp"
# include "utils.hpp"

# define EVENT_LIST_SIZE 100
# define BUFFER_SIZE 100000

enum EventType
{
    ERROR,
    IS_SERVER_SOCKET,
    READ_ERROR,
    READ_CONTINUE,
    READ_FINISH_REQUEST,
    READ_FINISH_FILE,
    SEND_RESPONSE,
    EDIT_FILE,
};

enum EventRecorderFlag
{
    ISREQUEST,
    ISFILE,
    RESPONSE_READY,
};

class KeventHandler
{
private:
    Http                                http_;
    std::vector<uintptr_t>              server_sockets_;
    std::map<int, EventRecorder>        fd_manager_;
    std::map< int, std::vector<char> >  fd_content_;
    std::vector<struct kevent>          change_list_;
    // event_list의 크기 생각해보기
    struct kevent                       event_list_[EVENT_LIST_SIZE];
    int                                 kq_;

    void    disconnectClient(int client_fd, std::map< int, std::vector<char> >& data);
    void    changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
                        uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
    void    initKqueue();
    void    createRequest(struct kevent* curr_event);
    void    createResponse(struct kevent* curr_event);
    void    sendResponse(struct kevent* curr_event);

public:
    KeventHandler(Http &http);
    ~KeventHandler();
    void openListenSocket();
    void setServerSocket(struct sockaddr_in *server_addr, Server server);
    bool createClientSocket(struct kevent* curr_event);
    int  getEventFlag(struct kevent* curr_event, char *buf, int *n);
    void socketError(struct kevent*  curr_event);
    void clientReadError(struct kevent* curr_event);
    void addContent(struct kevent* curr_event, char buf[], int n);
    bool isSocket(struct kevent* curr_event);
    int readFdFlag(struct kevent* curr_event, char *buf, int *n);
    int  writeFdFlag(struct kevent* curr_event);
    void runServer();
};

# endif
