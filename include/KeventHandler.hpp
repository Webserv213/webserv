#ifndef KEVENT_HANDLER_HPP
# define KEVENT_HANDLER_HPP

# include <dirent.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sys/event.h>
# include <sys/socket.h>
# include <sys/time.h>

# include "EventRecorder.hpp"
# include "Http.hpp"
# include "Utils.hpp"

# define EVENT_LIST_SIZE 500
# define CRLF_LENGTH 2

enum EventType
{
    ERROR,
    IS_SERVER_SOCKET,
    READ_ERROR,
    READ_FINISH_REQUEST,
    READ_FINISH_FILE,
    SEND_RESPONSE,
    CLOSE_CONNECTION,
    IDLE,
    WRITE_CGI,
    READ_CGI,
    DONE_CGI,
};

class KeventHandler
{
private:
    Http                    http_;
    std::vector<uintptr_t>  server_sockets_;

    std::map<int, EventRecorder>    fd_manager_;
    std::map<int, std::string>      fd_content_;

    std::map<std::string, std::string>  mime_type_;

    struct kevent               event_list_[EVENT_LIST_SIZE];
    std::vector<struct kevent>  change_list_;

    int kq_;

    // initial
    void    setMimeType();
    void    setServerSocket(struct sockaddr_in *server_addr, Server server);
    void    initKqueue();

    // utils
    void    changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
                            uint16_t flags, uint32_t fflags, intptr_t data, void *udata);
    void    setReadFileEvent(int curr_event_fd, int fd);

    // event setting
    bool    isSocket(struct kevent* curr_event);
    int     getEventFlag(struct kevent* curr_event);
    int     readFdFlag(struct kevent* curr_event);
    int     writeFdFlag(struct kevent* curr_event);

    // event
    void    executeCgi(struct kevent* curr_event);
    void    socketError(struct kevent* curr_event);
    bool    createClientSocket(struct kevent* curr_event);
    void    clientReadError(struct kevent* curr_event);
    void    executeMethod(int curr_event);
    void    createResponse(unsigned int cur_fd);
    void    sendResponse(unsigned int curr_event_fd, long write_able_buffer);
    void    disconnectClient(int client_fd);

    // pipe read
    int     isPipeFile(struct kevent* curr_event);

    // executeMethod utils
    int     transferFd(uintptr_t fd);
    int     getServerIndex(Request req);

    // location search
    int     getLocationIndex(Request req, Server &server, size_t *size);
    int     compareLocation(std::vector<std::string> t, std::vector<std::string> loc);
    bool    checkCgi(Request req, Location loc, std::string extension);

    // method handler
    void    methodGetHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size);
    void    methodPostHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size);
    void    methodDeleteHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size);

    // method handler utils
    bool        checkAccessMethod(std::string method, Location location);
    std::string createFilePath(Server &server, Request &req, int loc_idx, size_t size);


    // autoindex
    std::string makeDirList(std::string file_path);
    void        createResponseAutoindex(int curr_event_fd, std::string file_path);

    // request combine
    int     addSegmentReqAndReadMode(struct kevent* curr_event);
    int     readReqHeader(struct kevent* curr_event);
    int     readContentBody(struct kevent* curr_event);
    int     readChunkedBody(struct kevent* curr_event);
    void    readChunkedLength(struct kevent* curr_event);
    int     readChunkedData(struct kevent* curr_event);

    // request parsing
    void    parsingReqStartLineAndHeaders(struct kevent* curr_event);

    // error code
    void    forbidden403(int curr_event_fd);
    void    notFound404(int curr_event_fd);
    void    notAllowedMethod405(int curr_event_fd);
    void    requestEntityTooLarge413(int curr_event_fd);

    // method get utils
    void    getFaviconFile(int curr_event_fd);
    void    openFile(std::string file_path, int curr_event_fd);
    bool    isCgiRequest(int cur_fd, int idx, int loc_idx);
    bool    isRightMethod(Request &req, int cur_fd);

    // cgi setting
    char**  createEnv(int parent_fd);
    void    connectPipe(int parent_fd);
    void    closePipes(int parent_fd);

    // not use
    void        createFile(struct kevent* curr_event);
    void        createFileForPost(int curr_event_fd, std::string file_path);

public:
    KeventHandler(Http &http);
    ~KeventHandler();

    void    openListenSocket();
    void    runServer();
};

# endif