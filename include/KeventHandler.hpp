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
    std::vector<uintptr_t>  server_sockets_;        // 다중 서버 저장 벡터

    std::map<int, EventRecorder>    fd_manager_;    // FD 별 이벤트 관리 맵
    std::map<int, std::string>      fd_content_;    // FD 별 컨텐트 저장 맵 
    std::map<int, std::string>      cookies_;       // cookie 값 저장 맵

    std::map<std::string, std::string>  mime_type_; // mime type 저장 맵

    struct kevent               event_list_[EVENT_LIST_SIZE];
    std::vector<struct kevent>  change_list_;   // 이벤트 리스트 저장 벡터

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

    // parsing cgi_bla header
    std::string parsingCgiBlaHeader(const std::string& str);
    std::string parsingCgiSessionHeader(const std::string& str);
    
    // event
    void    writeBodyToCgi(struct kevent* curr_event);
    int     readBodyFromCgi(struct kevent* curr_event);
    void    socketError(struct kevent* curr_event);
    bool    createClientSocket(struct kevent* curr_event);
    void    clientReadError(struct kevent* curr_event);
    void    executeMethod(int curr_event_fd);
    void    createResponse(int curr_event_fd);
    void    sendResponse(int curr_event_fd, long writable_size);
    void    disconnectClient(int client_fd);

    // executeMethod utils
    int     transferFd(uintptr_t fd);
    int     getServerIndex(Request &req);

    // location search
    std::string getExtension(std::vector<std::string>& request_target);
    size_t      getLocationIndex(Request &req, Server &server, size_t *size);
    int         compareLocation(std::vector<std::string>& t, std::vector<std::string>& loc);
    bool        checkPostfix(Request &req, Location &loc, std::string &extension);

    // method handler
    void    methodGetHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size);
    void    methodPostHandler(Server &server, Request &req, int curr_event_fd, int loc_idx);
    void    methodDeleteHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size);

    // method handler utils
    bool        checkAccessMethod(std::string &method, Location &location);
    std::string createFilePath(Server &server, Request &req, int loc_idx, size_t size);

    // autoindex
    int         autoIndexStatus(Server &server, int loc_idx);
    std::string createDirecoryList(std::string& file_path);

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
    bool    isFaviconReq(Request &req);
    void    getFaviconFile(int curr_event_fd);
    void    openFile(std::string file_path, int curr_event_fd);
    bool    isCgiRequest(int curr_event_fd, int idx, int loc_idx);
    bool    isAvailableMethod(Request &req, int curr_event_fd);

    // cgi setting
    char    **createEnv(int parent_fd);
    void    connectPipe(int parent_fd);
    void    closePipes(int parent_fd);

    // cookie
    void    inputCookieHTML(int curr_event_fd);

public:
    KeventHandler(Http &http);
    ~KeventHandler();

    void    openListenSocket();
    void    runServer();
};

# endif