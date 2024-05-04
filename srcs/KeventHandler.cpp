#include "KeventHandler.hpp"

// 생성자
KeventHandler::KeventHandler(Http &http): http_(http)
{
    setMimeType();
}

// 소멸자
KeventHandler::~KeventHandler()
{

}

// 서버 Socket 생성 함수
void KeventHandler::openListenSocket()
{
    int listen_socket_fd;
    std::vector<Server> server;
    struct sockaddr_in server_addr;

    server = http_.getServer();

    for(size_t i = 0; i < server.size(); i++)
    {
        setServerSocket(&server_addr, server[i]);

        listen_socket_fd = socket(PF_INET, SOCK_STREAM, 0);

        EventRecorder st;
        fd_manager_[listen_socket_fd] = st;

        if (listen_socket_fd == -1)
            throw(std::runtime_error("socket() error\n"));

        if (bind(listen_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
            throw(std::runtime_error("bind() error\n"));

        if (listen(listen_socket_fd, EVENT_LIST_SIZE) == -1)
            throw(std::runtime_error("listen() error\n"));

        fcntl(listen_socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

        server_sockets_.push_back(listen_socket_fd);
        changeEvents(change_list_, listen_socket_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    }
}

// 서버 시작 함수
void KeventHandler::runServer(void)
{
    int             new_events;
    int             event_type;
    struct kevent*  curr_event;

    initKqueue();

    while (1)
    {
        new_events = kevent(kq_, &change_list_[0], change_list_.size(), event_list_, EVENT_LIST_SIZE, NULL);
        if (new_events == -1)
            std::cerr << "kevent() error!";

        change_list_.clear();

        for (int i = 0; i < new_events; ++i)
        {
            curr_event = &event_list_[i];
            event_type = getEventFlag(curr_event);

            switch(event_type)
            {
                case WRITE_CGI:
                    writeBodyToCgi(curr_event);
                    break ;

                case ERROR :
                    socketError(curr_event);
                    break ;

                case IS_SERVER_SOCKET :
                    createClientSocket(curr_event);
                    break ;

                case READ_ERROR :
                    clientReadError(curr_event);
                    break ;

                case READ_FINISH_REQUEST :
                    executeMethod(transferFd(curr_event->ident));
                    break ;

                case READ_FINISH_FILE :
                    createResponse(curr_event->ident);
                    break ;

                case SEND_RESPONSE :
                    sendResponse(curr_event->ident, curr_event->data);
                    break ;

                case CLOSE_CONNECTION :
                    disconnectClient(curr_event->ident);
                    break ;

                case IDLE :
                    break ;

                default :
                    std::cerr << "event exception error!\n";
            }
        }
    }
}

// mime-type 맵에 저장
void KeventHandler::setMimeType()
{
    mime_type_["html"] = "text/html";
    mime_type_["htm"] = "text/html";
    mime_type_["shtml"] = "text/html";
    mime_type_["css"] = "text/css";
    mime_type_["xml"] = "text/xml";
    mime_type_["rss"] = "text/xml";
    mime_type_["gif"] = "image/gif";
    mime_type_["jpeg"] = "image/jpeg";
    mime_type_["jpg"] = "image/jpeg";
    mime_type_["js"] = "application/x-javascript";
    mime_type_["txt"] = "text/plain";
    mime_type_["htc"] = "text/x-component";
    mime_type_["mml"] = "text/mathml";
    mime_type_["png"] = "image/png";
    mime_type_["ico"] = "image/x-icon";
    mime_type_["jng"] = "image/x-jng";
    mime_type_["wbmp"] = "image/vnd.wap.wbmp";
    mime_type_["jar"] = "application/java-archive";
    mime_type_["war"] = "application/java-archive";
    mime_type_["ear"] = "application/java-archive";
    mime_type_["hqx"] = "application/mac-binhex40";
    mime_type_["pdf"] = "application/pdf";
    mime_type_["cco"] = "application/x-cocoa";
    mime_type_["jardiff"] = "application/x-java-archive-diff";
    mime_type_["jnlp"] = "application/x-java-jnlp-file";
    mime_type_["run"] = "application/x-makeself";
    mime_type_["pl"] = "application/x-perl";
    mime_type_["pm"] = "application/x-perl";
    mime_type_["prc"] = "application/x-pilot";
    mime_type_["pdb"] = "application/x-pilot";
    mime_type_["rar"] = "application/x-rar-compressed";
    mime_type_["rpm"] = "application/x-redhat-package-manager";
    mime_type_["sea"] = "application/x-sea";
    mime_type_["swf"] = "application/x-shockwave-flash";
    mime_type_["sit"] = "application/x-stuffit";
    mime_type_["tcl"] = "application/x-tcl";
    mime_type_["tk"] = "application/x-tcl";
    mime_type_["der"] = "application/x-x509-ca-cert";
    mime_type_["pem"] = "application/x-x509-ca-cert";
    mime_type_["crt"] = "application/x-x509-ca-cert";
    mime_type_["xpi"] = "application/x-xpinstall";
    mime_type_["zip"] = "application/zip";
    mime_type_["deb"] = "application/octet-stream";
    mime_type_["bin"] = "application/octet-stream";
    mime_type_["exe"] = "application/octet-stream";
    mime_type_["dll"] = "application/octet-stream";
    mime_type_["dmg"] = "application/octet-stream";
    mime_type_["eot"] = "application/octet-stream";
    mime_type_["iso"] = "application/octet-stream";
    mime_type_["img"] = "application/octet-stream";
    mime_type_["msi"] = "application/octet-stream";
    mime_type_["msp"] = "application/octet-stream";
    mime_type_["msm"] = "application/octet-stream";
    mime_type_["mp3"] = "audio/mpeg";
    mime_type_["ra"] = "audio/x-realaudio";
    mime_type_["mpeg"] = "video/mpeg";
    mime_type_["mpg"] = "video/mpeg";
    mime_type_["mov"] = "video/quicktime";
    mime_type_["flv"] = "video/x-flv";
    mime_type_["avi"] = "video/x-msvideo";
    mime_type_["wmv"] = "video/x-ms-wmv";
    mime_type_["asx"] = "video/x-ms-asf";
    mime_type_["asf"] = "video/x-ms-asf";
    mime_type_["mng"] = "video/x-mng";
}

// 서버 소켓 구조체 초기화
void KeventHandler::setServerSocket(struct sockaddr_in *server_addr, Server server)
{
    memset(&(*server_addr), 0, sizeof(*server_addr));
    (*server_addr).sin_family = AF_INET;
    (*server_addr).sin_addr.s_addr = htonl(INADDR_ANY);
    (*server_addr).sin_port = htons(server.getListenPort());
}

// kqueue FD 생성
void KeventHandler::initKqueue()
{
    kq_ = kqueue();
    if (kq_ == -1)
        std::cerr << "kqueue() error!\n";
}

// 이벤트 변경(생성, 삭제 등)
void KeventHandler::changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
                                uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

// File을 open 후 Read 이벤트를 생성하는 함수
void KeventHandler::setReadFileEvent(int curr_event_fd, int file_fd)
{
    EventRecorder event_recorder(curr_event_fd);

    event_recorder.setEventReadFile(1);
    fd_content_[file_fd];
    
    fd_manager_[file_fd] = event_recorder;
    fd_manager_[curr_event_fd].setEventWriteRes(0);
    
    changeEvents(change_list_, file_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
}

// Socket FD 확인 함수
bool KeventHandler::isSocket(struct kevent* curr_event)
{
     for(size_t i = 0; i < server_sockets_.size(); i++)
    {
        if (curr_event->ident == server_sockets_[i])
            return (true);
    }
    return (false);
}

// Event flags 값을 확인하는 함수
int  KeventHandler::getEventFlag(struct kevent* curr_event)
{
    if (curr_event->flags & EV_ERROR)
    {
        return (IDLE);
    }
    if (curr_event->filter == EVFILT_TIMER)
        return (CLOSE_CONNECTION);
    if (curr_event->filter == EVFILT_READ)
    {
        if (isSocket(curr_event))
            return (IS_SERVER_SOCKET);
        return (readFdFlag(curr_event));
    }
    else if (curr_event->filter == EVFILT_WRITE)
        return (writeFdFlag(curr_event));
    return (IDLE);
}

// Read 이벤트인 FD를 처리하는 함수
int KeventHandler::readFdFlag(struct kevent* curr_event)
{
    char    buf[BUFFER_SIZE];
    ssize_t n = 0;

    if (fd_content_.find(curr_event->ident) != fd_content_.end())
    {
        n = read(curr_event->ident, buf, BUFFER_SIZE);
        if (n < 0 && (curr_event->flags & EV_EOF))
            return (CLOSE_CONNECTION);
        if (n < 0)
            return (READ_ERROR);
        fd_content_[curr_event->ident].append(buf, n);
        if (fd_manager_[curr_event->ident].getEventReadFile() == 0)
            return (addSegmentReqAndReadMode(curr_event));
        else
        {
            if (fd_manager_[curr_event->ident].getCgiStatus() == READ_CGI)
            {
                if (n == 0)
                {
                    fd_manager_[curr_event->ident].setCgiStatus(DONE_CGI);
                    fd_manager_[curr_event->ident].getSendCgiBody().clear();

                    return (READ_FINISH_REQUEST);
                }
                else
                    return (readBodyFromCgi(curr_event));
            }
            else
            {
                if (n == curr_event->data)
                    return (READ_FINISH_FILE);
                else    
                    return (IDLE);
            }
        }
    }
    return (IDLE);
}

// Write 이벤트인 FD를 처리하는 함수
int  KeventHandler::writeFdFlag(struct kevent* curr_event)
{
    std::map<int, std::string >::iterator content_iter = fd_content_.find(curr_event->ident);
    std::map<int, EventRecorder >::iterator manager_iter = fd_manager_.find(curr_event->ident);

    if (content_iter != fd_content_.end())
    {
        if (fd_content_[curr_event->ident].size() != 0 && fd_manager_[curr_event->ident].getEventWriteRes() == 1)
            return (SEND_RESPONSE);
    }
    else if (fd_manager_[curr_event->ident].getCgiStatus() == WRITE_CGI)
    {
        if (manager_iter != fd_manager_.end())
            return (WRITE_CGI);
        else
            return (ERROR);
    }
    return (IDLE);
}

// cgi_bla 헤더 파싱 함수
std::string KeventHandler::parsingCgiBlaHeader(const std::string& str)
{
    std::istringstream streamLine(str);
    std::string buf;

    std::getline(streamLine, buf);
    std::getline(streamLine, buf);
    std::getline(streamLine, buf);
    std::getline(streamLine, buf);

    return (buf);
}

// cgi_session 헤더 파싱 함수
std::string KeventHandler::parsingCgiSessionHeader(const std::string& str)
{
    std::istringstream streamLine(str);
    std::string buf;
    std::string body = "";

    std::getline(streamLine, buf);
    std::getline(streamLine, buf);
    std::getline(streamLine, buf);

    while (std::getline(streamLine, buf))
    {
        body += (buf + "\n");
    }

    return (body);
}

// cgi 프로세스로 전달 할 데이터를 쓰는 함수
void KeventHandler::writeBodyToCgi(struct kevent* curr_event)
{
    EventRecorder *parent = static_cast<EventRecorder *>(curr_event->udata);
    size_t curr_write_size = 0;

    if (parent->getSendCgiBody().size() > fd_manager_[curr_event->ident].getWriteBodyIndex() + curr_event->data)
        curr_write_size = curr_event->data;
    else
        curr_write_size = parent->getSendCgiBody().size() - fd_manager_[curr_event->ident].getWriteBodyIndex();

    ssize_t n = write(curr_event->ident, &(parent->getSendCgiBody().c_str()[fd_manager_[curr_event->ident].getWriteBodyIndex()]), curr_write_size);

    if (n == 0)
    {
        close(curr_event->ident);
        fd_manager_.erase(parent->getSendPipe(1));
        fd_content_.erase(parent->getSendPipe(1));
    }
    if (n < 0)
        std::cerr << "write body to cgi error!\n";

    fd_manager_[curr_event->ident].sumWriteBodyIndex(curr_write_size);
}

// cgi로부터 데이터를 저장하는 함수
int KeventHandler::readBodyFromCgi(struct kevent* curr_event)
{
    EventRecorder *event_recorder = static_cast<EventRecorder *>(curr_event->udata);

    if (event_recorder->getResponse().getBody() == "")
    {
        std::string cgi_file_extension = getExtension(event_recorder->getRequest().getRequestLine().getRequestTarget());

        if (cgi_file_extension == ".session")
            event_recorder->getResponse().setBody(parsingCgiSessionHeader(fd_content_[curr_event->ident]));
        else
            event_recorder->getResponse().addBody(parsingCgiBlaHeader(fd_content_[curr_event->ident]));
    }
    else
        event_recorder->getResponse().addBody(fd_content_[curr_event->ident]);

    fd_content_[curr_event->ident].clear();

    return (IDLE);
}

// socket error 확인 함수
void KeventHandler::socketError(struct kevent*  curr_event)
{
    for(size_t i = 0; i < server_sockets_.size(); i++)
    {
        if (curr_event->ident == server_sockets_[i])
            std::cerr << "server socket error!\n";
        else
        {
            std::cerr << "client socket error!\n";
            disconnectClient(curr_event->ident);
        }
    }
}

// 클라이언트 소켓 생성 함수
bool    KeventHandler::createClientSocket(struct kevent* curr_event)
{
    bool result = false;

    for(size_t i = 0; i < server_sockets_.size(); i++)
    {
        if (curr_event->ident == server_sockets_[i])
        {
            int client_socket = accept(server_sockets_[i], NULL, NULL);
            if (client_socket < 0)
                std::cerr << "create client error!\n";
            fcntl(client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    
            EventRecorder st;

            fd_manager_[client_socket] = st;
            fd_content_[client_socket];

            changeEvents(change_list_, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
            changeEvents(change_list_, client_socket, EVFILT_TIMER, EV_ADD, 0, 60000, NULL);
            
            result = true;
        }
    }
    return (result);
}

// client socket의 read 에러 발생 시 연결을 끊는 함수
void KeventHandler::clientReadError(struct kevent* curr_event)
{
    std::cerr << "client read error!\n";
    disconnectClient(curr_event->ident);
}

// GET, POST, DELETE Method 종류를 확인하고 알맞은 함수를 실행하는 함수
void    KeventHandler::executeMethod(int curr_event_fd)
{
    int idx;
    int loc_idx;
    size_t size = 0;
    Request &req = fd_manager_[curr_event_fd].getRequest();

    fd_content_[curr_event_fd].clear();
    idx = getServerIndex(req);
    loc_idx = getLocationIndex(req, http_.getServer()[idx], &size);

    if (isAvailableMethod(req, curr_event_fd))
        return ;
    if (isCgiRequest(curr_event_fd, idx, loc_idx))
        return ;

    if (req.getRequestLine().getMethod() == "GET")
        methodGetHandler(http_.getServer()[idx], req, curr_event_fd, loc_idx, size);
    else if (req.getRequestLine().getMethod() == "POST")
        methodPostHandler(http_.getServer()[idx], req, curr_event_fd, loc_idx);
    else if (req.getRequestLine().getMethod() == "DELETE")
        methodDeleteHandler(http_.getServer()[idx], req, curr_event_fd, loc_idx, size);
}

// Response 생성 함수
void KeventHandler::createResponse(int curr_event_fd)
{
    int parent_fd = fd_manager_[curr_event_fd].getParentFd();

    if (parent_fd == -1)
        parent_fd = curr_event_fd;

    std::string version = fd_manager_[parent_fd].getRequest().getRequestLine().getVersion();
    fd_manager_[parent_fd].getResponse().getStatusLine().setVersion(version);

    time_t rawTime;
    time(&rawTime);

    struct tm *timeInfo;
    timeInfo = gmtime(&rawTime);

    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
    fd_manager_[parent_fd].getResponse().getHeaders().setDate(buffer);

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size() != 0 && 
        fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget()[0] == "/favicon.ico")
        fd_manager_[parent_fd].getResponse().getHeaders().setContentType(mime_type_["ico"]);
    else
        fd_manager_[parent_fd].getResponse().getHeaders().setContentType(mime_type_["html"]);
    
    fd_manager_[parent_fd].getResponse().getHeaders().setLastModified("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setTransferEncoding("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setConnection("keep-alive");
    fd_manager_[parent_fd].getResponse().getHeaders().setContentEncoding("default");

    if (parent_fd != curr_event_fd)
    {
        fd_manager_[parent_fd].getResponse().addBody(fd_content_[curr_event_fd]);
        if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size() != 0 && 
            fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget()[0] == "/favicon.ico")
            fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(numToString(1150));
        else
            fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(numToString(fd_content_[curr_event_fd].size()));
    }
    else
        fd_manager_[curr_event_fd].getResponse().getHeaders().setContentLength(numToString(fd_manager_[curr_event_fd].getResponse().getBody().size()));

    fd_manager_[parent_fd].getResponse().getStatusLine().setVersion("HTTP/1.1");
    fd_content_[parent_fd] += (fd_manager_[parent_fd].getResponse().getStatusLine().getVersion() + " ");
    fd_content_[parent_fd] += (fd_manager_[parent_fd].getResponse().getStatusLine().getStatusCode() + " ");
    fd_content_[parent_fd] += (fd_manager_[parent_fd].getResponse().getStatusLine().getStatusText() + "\r\n");
    fd_content_[parent_fd] += ("Date: " + fd_manager_[parent_fd].getResponse().getHeaders().getDate() + "\r\n");
    fd_content_[parent_fd] += ("Content-Type: " + fd_manager_[parent_fd].getResponse().getHeaders().getContentType() + "\r\n");
    fd_content_[parent_fd] += ("Content-Length: " + fd_manager_[parent_fd].getResponse().getHeaders().getContentLength() + "\r\n");
    fd_content_[parent_fd] += ("Connection: " + fd_manager_[parent_fd].getResponse().getHeaders().getConnection()+ "\r\n");

    if (fd_manager_[parent_fd].getResponse().getHeaders().getCookieId() != "")
        fd_content_[parent_fd] += ("Set-Cookie: " + fd_manager_[parent_fd].getResponse().getHeaders().getCookieId() + "\r\n");

    fd_content_[parent_fd] += "\r\n" + fd_manager_[parent_fd].getResponse().getBody();

    if (parent_fd != curr_event_fd)
    {
        close(curr_event_fd);
        fd_content_.erase(curr_event_fd);
        fd_manager_.erase(curr_event_fd);
    }

    fd_manager_[parent_fd].setEventWriteRes(1);
    changeEvents(change_list_, parent_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
}

// 생성된 Response를 클라이언트로 보내는 함수
void    KeventHandler::sendResponse(int curr_event_fd, long writable_size)
{
    size_t curr_write_size = 0;

    if (fd_content_[curr_event_fd].size() > fd_manager_[curr_event_fd].getWriteBodyIndex() + writable_size)
        curr_write_size = writable_size;
    else
        curr_write_size = fd_content_[curr_event_fd].size() - fd_manager_[curr_event_fd].getWriteBodyIndex();
 
    if (curr_write_size == 0)
    {
        EventRecorder n;

        fd_manager_[curr_event_fd] = n;
        fd_content_[curr_event_fd].clear();

        changeEvents(change_list_, curr_event_fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        changeEvents(change_list_, curr_event_fd, EVFILT_TIMER, EV_ADD, 0, 60000, NULL);
        
        return ;
    }

    int n = write(curr_event_fd, &(*(fd_content_[curr_event_fd].begin() + fd_manager_[curr_event_fd].getWriteBodyIndex())), curr_write_size);

    if (n < 0)
    {
        std::cerr << "client socket write error!\n";
        disconnectClient(curr_event_fd);
        return ;
    }

    fd_manager_[curr_event_fd].sumWriteBodyIndex(n);
}

// 클라이언트와의 연결을 끊는 함수
void    KeventHandler::disconnectClient(int client_fd)
{   
    changeEvents(change_list_, client_fd, EVFILT_TIMER, EV_DELETE, 0, 0, NULL);
    close(client_fd);
    fd_manager_.erase(client_fd);
    fd_content_.erase(client_fd);
}

// 자식 FD를 부모 FD로 변환하는 함수
int KeventHandler::transferFd(uintptr_t fd)
{
    // 파이프 FD면 부모 FD 리턴
    if (fd_manager_[fd].getCgiStatus() == DONE_CGI)
    {
        int parent_fd = fd_manager_[fd].getParentFd();

        fd_manager_[parent_fd].setCgiStatus(DONE_CGI);
        fd_manager_.erase(fd);
        fd_content_.erase(fd);

        return (parent_fd);
    }
    // 아니면 원래 FD 리턴
    return (fd);
}

// 서버가 하나 이상일 때 서버의 인덱스를 확인하는 함수
int KeventHandler::getServerIndex(Request &req)
{
    for (size_t i = 0; i < http_.getServer().size(); i++)
    {
        if (req.getHeaders().getListenPort() == http_.getServer()[i].getListenPort() &&
            req.getHeaders().getHost() == http_.getServer()[i].getServerName())
            return (i);
    }
    return (0);
}

// 확장자만 잘라오는 함수
std::string KeventHandler::getExtension(std::vector<std::string>& request_target)
{
    std::string cgi_file_extension;
    size_t size = 0;

    // 확장자 검사(가장 마지막 확장자를 확장자로)
    for(int i = request_target.size() - 1; i >= 0; i--)
    {
        if (request_target[i].rfind('.') != std::string::npos)
        {
            size = request_target[i].rfind('.');
            cgi_file_extension = request_target[i].substr(size);
            break ;
        }
    }

    return (cgi_file_extension);
}

// 각 Location 별로 인덱스를 확인하는 함수
size_t KeventHandler::getLocationIndex(Request &req, Server &server, size_t *res_same_path_cnt)
{
    size_t index = 0;
    size_t same_path_cnt;
    std::vector<std::string> request_target = req.getRequestLine().getRequestTarget();
    std::string cgi_file_extension = getExtension(request_target);

    //알맞는 location 파싱
    for (size_t i = 1; i < server.getLocation().size(); i++)
    {
        same_path_cnt = compareLocation(request_target, server.getLocation()[i].getUrlPostfix());
        if (checkPostfix(req, server.getLocation()[i], cgi_file_extension))
        {
            *res_same_path_cnt = 1;
            return (i);
        }
        if (same_path_cnt == server.getLocation()[i].getUrlPostfix().size())
        {
            *res_same_path_cnt = same_path_cnt;
            return (i);
        }
        if (*res_same_path_cnt < same_path_cnt)
        {
            *res_same_path_cnt = same_path_cnt;
            index = i;
        }
    }
    if (request_target.size() < server.getLocation()[index].getUrlPostfix().size())
        return (0);
    return (index);
}

// URL 로케이션의 길이를 비교하는 함수
int KeventHandler::compareLocation(std::vector<std::string>& t, std::vector<std::string>& loc)
{
    size_t i = 0;

    while (i < t.size())
    {
        if (i + 1 > loc.size())
            return (i);

        if (t[i] != loc[i])
            return (i);
        i++;
    }
    return (i);
}

// 확장자 검사 함수
bool KeventHandler::checkPostfix(Request &req, Location &loc, std::string &extension)
{
    if (loc.getUrlPostfix().size() == 1 && loc.getUrlPostfix()[0] == extension && req.getRequestLine().getMethod() == "POST")
        return (true);
    else if (loc.getUrlPostfix().size() == 1 && loc.getUrlPostfix()[0] == extension && extension == ".session" && req.getRequestLine().getMethod() == "GET")
        return (true);
    return (false);
}

// Get Method 처리 함수
void KeventHandler::methodGetHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size)
{
    std::string file_path;
    bool        is_allow_method;
    int         file_type;

    if (isFaviconReq(req))
        getFaviconFile(curr_event_fd);
    else
    {
        is_allow_method = checkAccessMethod(req.getRequestLine().getMethod(), server.getLocationBlock(loc_idx));
        if (is_allow_method == true)
        {
            file_path = createFilePath(server, req, loc_idx, size);
            file_type = isFileOrDirectory(file_path.c_str());

            if (file_type == IS_DIR)
            {
                if (autoIndexStatus(server, loc_idx) == OFF)
                {
                    file_path += "/" + server.getLocationBlock(loc_idx).getIndex();
                    openFile(file_path, curr_event_fd);
                }
                else
                {
                    std::string dir_list_html = createDirecoryList(file_path);

                    fd_manager_[curr_event_fd].getResponse().getHeaders().setContentLength(numToString(dir_list_html.size()));
                    fd_manager_[curr_event_fd].getResponse().addBody(dir_list_html);

                    createResponse(curr_event_fd);
                }
            }
            else if (file_type == IS_FILE)
                openFile(file_path, curr_event_fd);
            else if (file_type == FILE_NOT_FOUND)
                notFound404(curr_event_fd);
        }
        else
            notAllowedMethod405(curr_event_fd);
    }
    changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
}

// Post Method 처리 함수
void KeventHandler::methodPostHandler(Server &server, Request &req, int curr_event_fd, int loc_idx)
{
    bool    is_allow_method = checkAccessMethod(req.getRequestLine().getMethod(), server.getLocationBlock(loc_idx));

    if (server.getLocationBlock(loc_idx).getClientMaxBodySize() > -1)
    {
        size_t  size_t_client_max_body = static_cast<size_t>(server.getLocationBlock(loc_idx).getClientMaxBodySize());
        
        if (fd_manager_[curr_event_fd].getRequest().getBody().size() > size_t_client_max_body)
        {
            requestEntityTooLarge413(curr_event_fd);
            return ;
        }
    }

    if (is_allow_method == true)
        createResponse(curr_event_fd);
    else
        notAllowedMethod405(curr_event_fd);
}

// Delete Method 처리 함수
void    KeventHandler::methodDeleteHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size)
{
    struct stat st;
    bool        is_allow_method = checkAccessMethod(req.getRequestLine().getMethod(), server.getLocationBlock(loc_idx));

    if (is_allow_method == true)
    {
        std::string file_path = createFilePath(server, req, loc_idx, size);

        if (stat(file_path.c_str(), &st) != 0)
            notFound404(curr_event_fd);
        else
        {
            if (std::remove(file_path.c_str()) == 0)
                createResponse(curr_event_fd);
            else
                forbidden403(curr_event_fd);
        }
    }
    else
        notAllowedMethod405(curr_event_fd);
}

// 해당되는 Location에서 허용하는 Method인지 확인하는 함수
bool KeventHandler::checkAccessMethod(std::string &method, Location &location)
{
    bool exist = false;

    for (size_t i = 0; i < location.getAccessMethod().size(); i++)
    {
        if (location.getAccessMethod()[i] == method)
            exist = true;
    }
    return (exist);
}

// open 하려는 file 경로를 만드는 함수
std::string KeventHandler::createFilePath(Server &server, Request &req, int loc_idx, size_t size)
{
    std::string file_path = server.getLocationBlock(loc_idx).getRoot();

    for (size_t i = 0; i < req.getRequestLine().getRequestTarget().size(); i++)
    {
        if (i < size)
            continue ;
        file_path += "/";
        file_path += req.getRequestLine().getRequestTarget()[i];
    }

    return (file_path);
}

// autoindex 값 확인 함수
int KeventHandler::autoIndexStatus(Server &server, int loc_idx)
{
    if (server.getLocationBlock(loc_idx).getAutoIndex() == true)
        return (ON);
    return (OFF);
}

// autoindex 요청 시 디렉토리에 들어있는 파일과 폴더명에 대한 html을 만드는 함수
std::string KeventHandler::createDirecoryList(std::string& file_path)
{
    DIR* dir = opendir(file_path.c_str());
    if (!dir) {
        // 디렉토리 열기 실패
        std::cerr << "Error: Unable to open directory!\n";
    }

    std::string result = "<!DOCTYPE html>\n";
    result += "<html>\n";
    result += "<head>\n";
    result += "<title>Directory Listing</title>\n";
    result += "</head>\n";
    result += "<body>\n";
    result += "<h1>Directory Listing</h1>\n";
    result += "<ul>\n";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            // Check if it's a file or directory
            std::string path = file_path + "/" + name;
            std::string type = (isDirectory(path)) ? "[DIR]" : "[FILE]";
            result += "<li>" + type + " - " + name + "</li>\n";
        }
    }

    result += "</ul>\n";
    result += "</body>\n";
    result += "</html>\n";
    
    closedir(dir);
    return (result);
}

// 나누어져서 들어오는 Request 메시지를 처리하고 Body 형식이 Chunked, Content-length에 따라서 그에 적합한 처리를 수행하는 함수
int KeventHandler::addSegmentReqAndReadMode(struct kevent* curr_event)
{
    int res = IDLE;

    if (fd_manager_[curr_event->ident].getReadType() == READ_HEADER)
    {
        res = readReqHeader(curr_event);
        if (res == IDLE)
            return (res);
    }

    if (fd_manager_[curr_event->ident].getReadType() == NO_EXIST_BODY)
        res = READ_FINISH_REQUEST;
    else if (fd_manager_[curr_event->ident].getReadType() == READ_CONTENT_LENGTH_BODY)
        res = readContentBody(curr_event);
    else if (fd_manager_[curr_event->ident].getReadType() == READ_CHUNKED_BODY)
        res = readChunkedBody(curr_event);
    
    return (res);
}

// 나누어져서 들어오는 Request 메시지를 조립하는 함수
int KeventHandler::readReqHeader(struct kevent* curr_event)
{
    int     read_req_header_res = IDLE;
    size_t  i = fd_manager_[curr_event->ident].getFdContentIndex();
    std::string::size_type n = fd_content_[curr_event->ident].find("\r\n\r\n", i);

    if (n != std::string::npos)
    {
        fd_manager_[curr_event->ident].setFdContentIndex(n + CRLF_LENGTH + CRLF_LENGTH);
        fd_manager_[curr_event->ident].setHeaderCrLf(1);
    }

    if (fd_manager_[curr_event->ident].getHeaderCrLf() == 1)
    {
        parsingReqStartLineAndHeaders(curr_event);
        fd_manager_[curr_event->ident].setHeaderCrLf(0);
        read_req_header_res = fd_manager_[curr_event->ident].getReadType();
    }

    return (read_req_header_res);
}

// Content-length 타입의 Body를 읽는 함수
int KeventHandler::readContentBody(struct kevent* curr_event)
{
    int     read_content_body_res = IDLE;
    size_t  total_content_body_length = fd_manager_[curr_event->ident].getRequest().getHeaders().getContentLength();

    if (total_content_body_length == 0)
        read_content_body_res = READ_FINISH_REQUEST;
    else
    {
        if (fd_content_[curr_event->ident].size() - fd_manager_[curr_event->ident].getFdContentIndex() >= total_content_body_length)
        {
            std::string::iterator content_iter = fd_content_[curr_event->ident].begin() + fd_manager_[curr_event->ident].getFdContentIndex();
            std::string           content_split(content_iter, content_iter + total_content_body_length);

            fd_manager_[curr_event->ident].getRequest().setBody(content_split);

            read_content_body_res = READ_FINISH_REQUEST;
        }
    }

    if (read_content_body_res == READ_FINISH_REQUEST)
    {
        fd_manager_[curr_event->ident].setFdContentIndex(0);
    }

    return (read_content_body_res);
}

// Chunked 타입의 Body를 읽는 함수
int KeventHandler::readChunkedBody(struct kevent* curr_event)
{
    int read_chunked_body_res = IDLE;

    while (1)
    {
        size_t  i = fd_manager_[curr_event->ident].getFdContentIndex();
        std::string::size_type n = fd_content_[curr_event->ident].find("\r\n", i);

        if (n != std::string::npos)
        {
            fd_manager_[curr_event->ident].setFdContentIndex(n + CRLF_LENGTH);
            fd_manager_[curr_event->ident].setChunkedCrLf(1);
            if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_DATA)
                fd_manager_[curr_event->ident].setChunkedCurrentReadLength(fd_manager_[curr_event->ident].getFdContentIndex() - CRLF_LENGTH - fd_manager_[curr_event->ident].getChunkedBeginIndex());
            if (fd_manager_[curr_event->ident].getChunkedCurrentReadLength() < fd_manager_[curr_event->ident].getChunkedTotalReadLength())
                fd_manager_[curr_event->ident].setChunkedCrLf(0);
        }
        else
            break ;
        
        if (fd_manager_[curr_event->ident].getChunkedCrLf() == 1)
        {
            if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_LENGTH)
                readChunkedLength(curr_event);
            else if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_DATA)
                read_chunked_body_res = readChunkedData(curr_event);
        }
        
        if (read_chunked_body_res == READ_FINISH_REQUEST)
            break ;
        // fd_content_에 읽을 문자의 수가 chunked length 보다 작고 crlf를 만나지 못했을 때 탈출하는 조건
        if (fd_manager_[curr_event->ident].getFdContentIndex() == fd_content_[curr_event->ident].size())
            break ;
    }

    return (read_chunked_body_res);
}

// Chunked Length를 읽는 함수
void    KeventHandler::readChunkedLength(struct kevent* curr_event)
{
    std::string::iterator   content_iter = fd_content_[curr_event->ident].begin() + fd_manager_[curr_event->ident].getChunkedBeginIndex();
    size_t                  last_pos = fd_manager_[curr_event->ident].getFdContentIndex() - fd_manager_[curr_event->ident].getChunkedBeginIndex() - CRLF_LENGTH;
    std::string             chunked_length(content_iter, content_iter + last_pos);
    int                     chunked_length_decimal = hexToDecimal(chunked_length.c_str(), chunked_length.size());

    fd_manager_[curr_event->ident].setChunkedTotalReadLength(chunked_length_decimal);
    fd_manager_[curr_event->ident].setChunkedDataType(CHUNKED_DATA);
    fd_manager_[curr_event->ident].setChunkedCrLf(0);
    fd_manager_[curr_event->ident].setChunkedBeginIndex(fd_manager_[curr_event->ident].getFdContentIndex());
}

// Chunked Data를 읽는 함수
int KeventHandler::readChunkedData(struct kevent* curr_event)
{
    int read_chunked_data_res = IDLE;

    // chunked data 끝
    if (fd_manager_[curr_event->ident].getChunkedTotalReadLength() == 0 && fd_manager_[curr_event->ident].getChunkedCrLf() == 1)
    {
        fd_manager_[curr_event->ident].setFdContentIndex(0);
        read_chunked_data_res = READ_FINISH_REQUEST;
    }
    else
    {
        std::string::iterator   content_iter = fd_content_[curr_event->ident].begin() + fd_manager_[curr_event->ident].getChunkedBeginIndex();
        size_t                  last_pos = fd_manager_[curr_event->ident].getFdContentIndex() - fd_manager_[curr_event->ident].getChunkedBeginIndex() - CRLF_LENGTH;
        std::string             chunked_data(content_iter, content_iter + last_pos);

        fd_manager_[curr_event->ident].getRequest().addBody(chunked_data);
        fd_manager_[curr_event->ident].setChunkedDataType(CHUNKED_LENGTH);
        fd_manager_[curr_event->ident].setChunkedCrLf(0);
        fd_manager_[curr_event->ident].setChunkedBeginIndex(fd_manager_[curr_event->ident].getFdContentIndex());
        fd_manager_[curr_event->ident].setChunkedCurrentReadLength(0);
        fd_manager_[curr_event->ident].setChunkedTotalReadLength(0);
    }

    return (read_chunked_data_res);
}

// 완성된 Request의 Header, Start Line 부분을 파싱하는 함수
void    KeventHandler::parsingReqStartLineAndHeaders(struct kevent* curr_event)
{
    Request req;
    std::string buf;
    std::string key;
    std::string value;
    std::istringstream full_content(fd_content_[curr_event->ident], fd_manager_[curr_event->ident].getFdContentIndex());

    // start line
    std::getline(full_content, buf, ' ');
    req.getRequestLine().setMethod(buf);
    std::getline(full_content, buf, ' ');
    req.getRequestLine().setRequestTarget(buf);
    std::getline(full_content, buf);
    req.getRequestLine().setVersion(buf);

    // headers
    while (std::getline(full_content, buf))
    {
        std::istringstream line(buf);

        std::getline(line, key, ' ');
        std::getline(line, value);

        if (key == "Host:")
            req.getHeaders().setFullPath(value);
        else if (key == "Cookie:")
            req.getHeaders().setCookie(value);
        else if (key == "Accept:")
            req.getHeaders().setAccept(value);
        else if (key == "Accept-Encoding:")
            req.getHeaders().setAcceptEncoding(value);
        else if (key == "Accept-Language:")
            req.getHeaders().setAcceptLanguage(value);
        else if (key == "Connection:")
            req.getHeaders().setConnection(value);
        else if (key == "Upgrade-Insecure-Requests:")
            req.getHeaders().setUpgradeInsecureRequests(value);
        else if (key == "User-Agent:")
            req.getHeaders().setUserAgent(value);
        else if (key == "X-Secret-Header-For-Test:")
            req.getHeaders().setXSecretHeaderForTest(value);
        else if (key == "Content-Length:")
        {
            int length = std::atoi(value.c_str());
            req.getHeaders().setContentLength(length);
            fd_manager_[curr_event->ident].setReadType(READ_CONTENT_LENGTH_BODY);
        }
        else if (key == "Transfer-Encoding:")
        {
            req.getHeaders().setTransferEncoding(value);
            fd_manager_[curr_event->ident].setChunkedBeginIndex(fd_manager_[curr_event->ident].getFdContentIndex());
            fd_manager_[curr_event->ident].setReadType(READ_CHUNKED_BODY);
        }
    }
    if (fd_manager_[curr_event->ident].getReadType() != READ_CONTENT_LENGTH_BODY
        && fd_manager_[curr_event->ident].getReadType() != READ_CHUNKED_BODY)
    {
        fd_manager_[curr_event->ident].setReadType(NO_EXIST_BODY);
    }

    fd_manager_[curr_event->ident].setRequest(req);
}

// 403 error html 파일 오픈 함수
void KeventHandler::forbidden403(int curr_event_fd)
{
    int fd;

    fd = open("./var/www/error/error_403.html", O_RDONLY);
    if (fd < 0)
        std::cerr << "403 html open error!";
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("403");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("Forbidden");

    setReadFileEvent(curr_event_fd, fd);
}

// 404 error html 파일 오픈 함수
void KeventHandler::notFound404(int curr_event_fd)
{
    int fd;

    fd = open("./var/www/error/error_404.html", O_RDONLY);
    if (fd < 0)
        std::cerr << "404 html open error!";
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("404");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("Not Found");

    setReadFileEvent(curr_event_fd, fd);
}

// 405 error html 파일 오픈 함수
void KeventHandler::notAllowedMethod405(int curr_event_fd)
{
    int fd;

    fd = open ("./var/www/error/error_405.html", O_RDONLY);
    if (fd < 0)
        std::cerr << "405 html open error!";
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("405");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("Method Not Allowed");

    setReadFileEvent(curr_event_fd, fd);
}

// 413 error html 파일 오픈 함수
void KeventHandler::requestEntityTooLarge413(int curr_event_fd)
{
    int fd;

    fd = open ("./var/www/error/error_413.html", O_RDONLY);
    if (fd < 0)
        std::cerr << "413 html open error!";
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("413");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("Request Entity Too Large");

    setReadFileEvent(curr_event_fd, fd);
}

// favicon에 대한 요청인지 확인하는 함수
bool KeventHandler::isFaviconReq(Request &req)
{
    std::vector<std::string> request_target;

    request_target = req.getRequestLine().getRequestTarget();

    if (request_target.size() != 0 && request_target[0] == "favicon.ico")
        return (true);

    return (false);
}

// favicion 파일 오픈 및 이벤트를 생성하는 함수
void KeventHandler::getFaviconFile(int curr_event_fd)
{
    int fd;

    fd = open ("./var/www/favicon.ico", O_RDONLY);
    if (fd < 0)
        notFound404(curr_event_fd);
    else
    {
        fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        setReadFileEvent(curr_event_fd, fd);
    }
}

// file 오픈 및 함수
void KeventHandler::openFile(std::string file_path, int curr_event_fd)
{
    int fd;
    struct stat st;

    fd = open (file_path.c_str(), O_RDONLY);
    if (fd < 0)
        notFound404(curr_event_fd);
    else
    {
        stat(file_path.c_str(), &st);
        if (st.st_size == 0)
        {
            EventRecorder event_recorder(curr_event_fd);
            fd_manager_[fd] = event_recorder;
            createResponse(fd);
            close(fd);
            return ;
        }
        fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        setReadFileEvent(curr_event_fd, fd);
    }
}

// cgi 요청인지 확인하는 함수
bool    KeventHandler::isCgiRequest(int curr_event_fd, int idx, int loc_idx)
{
    int send_fd[2];
    int recv_fd[2];
    std::string cgi_file_extension = getExtension(fd_manager_[curr_event_fd].getRequest().getRequestLine().getRequestTarget());

    if (fd_manager_[curr_event_fd].getCgiStatus() != DONE_CGI && http_.getServer()[idx].getLocationBlock(loc_idx).getCgiPath() != "")
    {
        fd_manager_[curr_event_fd].setSendCgiBody(fd_manager_[curr_event_fd].getRequest().getBody());
        // session을 생성하는 요청일 때
        if (cgi_file_extension == ".session")
        {
            /**
             * cookie에 null 들어있으면
             * session_id 생성하고, data 저장
             * response의 set-cookie 헤더에 session_id 담아주기
            */
            // cookie가 없어서 클아이언트로부터 cookie 값을 post 받았을 때
            if (fd_manager_[curr_event_fd].getRequest().getRequestLine().getMethod() == "POST")
            {

                time_t rawTime;
                time(&rawTime);

                struct tm *timeInfo;
                timeInfo = gmtime(&rawTime);

                char buffer[80];
                strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);

                int         session_id;
                std::string cookie_id = "session_id=";

                // session_id 생성
                session_id = customHash(buffer);

                // cookie data 저장
                cookies_[session_id] = fd_manager_[curr_event_fd].getRequest().getBody();

                // response set-cookie 설정
                cookie_id += numToString(session_id);
                fd_manager_[curr_event_fd].getResponse().getHeaders().setCookieId(cookie_id);

                // cookie 값을 cgi로 보냄
                fd_manager_[curr_event_fd].setSendCgiBody(cookies_[session_id]);
            }
            // 요청 메세지에 cookie가 있을 때 cookies_ 맵에 있는지 확인하고 없으면 inputCookieHTML 반환
            else if (fd_manager_[curr_event_fd].getRequest().getHeaders().getCookie() != "")
            {
                std::string session_id_str;

                std::istringstream cookie_value(fd_manager_[curr_event_fd].getRequest().getHeaders().getCookie());
                std::getline(cookie_value, session_id_str, '=');
                std::getline(cookie_value, session_id_str);

                int session_id_int;
                session_id_int = strtod(session_id_str.c_str(), NULL);

                if (cookies_.find(session_id_int) != cookies_.end())
                {
                    std::string data = cookies_[session_id_int];
                    fd_manager_[curr_event_fd].setSendCgiBody(data);
                }
                else
                {
                    inputCookieHTML(curr_event_fd);
                    return (true);
                }
            }
            // cookie가 없을 때
            else
            {
                // response에 html 입력창을 반환하도록 해줘야 함
                inputCookieHTML(curr_event_fd);
                return (true);
            }
        }

        fd_manager_[curr_event_fd].getRequest().setBody("");

        EventRecorder pipe_event_recorder(curr_event_fd);

        pipe(send_fd);
        fcntl(send_fd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        fcntl(send_fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        fd_manager_[send_fd[1]] = pipe_event_recorder;
        fd_manager_[send_fd[0]] = pipe_event_recorder;

        fd_manager_[curr_event_fd].setSendPipe(0, send_fd[0]);
        fd_manager_[curr_event_fd].setSendPipe(1, send_fd[1]);
        fd_manager_[curr_event_fd].setCgiPath(http_.getServer()[idx].getLocationBlock(loc_idx).getCgiPath());

        pipe(recv_fd);
        fcntl(recv_fd[1], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        fcntl(recv_fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        fd_manager_[recv_fd[1]] = pipe_event_recorder;
        fd_manager_[recv_fd[0]] = pipe_event_recorder;

        fd_manager_[curr_event_fd].setReceivePipe(0, recv_fd[0]);
        fd_manager_[curr_event_fd].setReceivePipe(1, recv_fd[1]);

        int pid = fork();
        //child
        if (pid == 0)
        {
            char **env;
            char *path[2];

            env = createEnv(curr_event_fd);
            path[0] = const_cast<char *>(fd_manager_[curr_event_fd].getCgiPath().c_str());
            path[1] = 0;

            connectPipe(curr_event_fd);

            if (execve(const_cast<char *>(fd_manager_[curr_event_fd].getCgiPath().c_str()), path, env) < 0)
                std::cerr << "execve error!\n";
        }
        //parent
        else
        {
            closePipes(curr_event_fd);
            changeEvents(change_list_, curr_event_fd , EVFILT_READ, EV_DELETE, 0, 0, NULL);

            fd_manager_[fd_manager_[curr_event_fd].getSendPipe(1)].setCgiStatus(WRITE_CGI);
            changeEvents(change_list_, fd_manager_[curr_event_fd].getSendPipe(1), EVFILT_WRITE, EV_ADD, 0, 0, &fd_manager_[curr_event_fd]);

            fd_manager_[fd_manager_[curr_event_fd].getReceivePipe(0)].setCgiStatus(READ_CGI);
            fd_manager_[fd_manager_[curr_event_fd].getReceivePipe(0)].setEventReadFile(1);
            fd_content_[fd_manager_[curr_event_fd].getReceivePipe(0)];
            changeEvents(change_list_, fd_manager_[curr_event_fd].getReceivePipe(0), EVFILT_READ, EV_ADD, 0, 0, &fd_manager_[curr_event_fd]);
        }
        return (true);
    }
    return (false);
}

// 허용 가능한 Method 요청인지 확인하는 함수
bool    KeventHandler::isAvailableMethod(Request &req, int curr_event_fd)
{
    std::string cur_method;

    cur_method = req.getRequestLine().getMethod();

    if ((cur_method == "GET" || cur_method == "POST" || cur_method == "DELETE") == false)
    {
        notAllowedMethod405(curr_event_fd);
        changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        return (true);
    }

    return (false);
}

// cgi 프로세스를 fork 할 때 execve 함수의 환경변수를 생성하는 함수
char** KeventHandler::createEnv(int parent_fd)
{
    char **env = new char *[6];

    std::string str0 = "REQUEST_METHOD=" + fd_manager_[parent_fd].getRequest().getRequestLine().getMethod();
    std::string str1 = "SERVER_PROTOCOL=" + fd_manager_[parent_fd].getRequest().getRequestLine().getVersion();

    // create PATH_INFO
    size_t i;
    for(i = fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size() - 1; i >= 0; i--)
    {
        if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget()[i].rfind('.') != std::string::npos)
        {
            i++;
            break ;
        }
    }

    std::string str2 = "PATH_INFO=";
    for(; i < fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size(); i++)
    {
        str2 += fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget()[i];
        if (i != fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size() - 1)
            str2 += "/";
    }
    if (str2 == "PATH_INFO=")
        str2 += "/";

    std::string str3 = "CONTENT_LENGTH=" + std::to_string(fd_manager_[parent_fd].getRequest().getBody().size());
    std::string str4 = "HTTP_X_SECRET_HEADER_FOR_TEST=" + fd_manager_[parent_fd].getRequest().getHeaders().getXSecretHeaderForTest();

    env[0] = new char[str0.size() + 1];
    strcpy(env[0], str0.c_str());
    env[1] = new char[str1.size() + 1];
    strcpy(env[1], str1.c_str());
    env[2] = new char[str2.size() + 1];
    strcpy(env[2], str2.c_str());
    env[3] = new char[str3.size() + 1];
    strcpy(env[3], str3.c_str());
    env[4] = new char[str4.size() + 1];
    strcpy(env[4], str4.c_str());
    env[5] = NULL;

    return (env);
}

// cgi 프로세스를 fork 할 때 자식의 stdin, stdout 으로 dup 하고 안쓰는 pipe를 close 하는 함수
void KeventHandler::connectPipe(int parent_fd)
{
    dup2(fd_manager_[parent_fd].getSendPipe(0), 0);
    dup2(fd_manager_[parent_fd].getReceivePipe(1), 1);

    close(fd_manager_[parent_fd].getSendPipe(1));
    close(fd_manager_[parent_fd].getReceivePipe(0));
}

// cgi 프로세스를 fork 할 때 사용하지 않는 pipe를 닫는 함수
void KeventHandler::closePipes(int parent_fd)
{
    close(fd_manager_[parent_fd].getSendPipe(0));
    fd_manager_.erase(fd_manager_[parent_fd].getSendPipe(0));

    close(fd_manager_[parent_fd].getReceivePipe(1));
    fd_manager_.erase(fd_manager_[parent_fd].getReceivePipe(1));
}

// cookie를 요청하는 html을 오픈하고 이벤트를 생성하는 함수
void KeventHandler::inputCookieHTML(int curr_event_fd)
{
    int fd;

    fd = open ("./var/www/session/inputCookie.html", O_RDONLY);
    if (fd < 0)
        std::cerr << "inputCookieHTML open error!\n";
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("200");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("OK");

    setReadFileEvent(curr_event_fd, fd);
}
