#include "KeventHandler.hpp"
#include <dirent.h>

KeventHandler::KeventHandler(Http &http): http_(http)
{
    setMimeType();
}

KeventHandler::~KeventHandler()
{

}

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

void    checkRequest(Request &req)
{
    // std::cout << "=====start_line=====\n";
    std::cout << "Host: " << req.getHeaders().getHost() << "\n";
    std::cout << "Listen socket: " << req.getHeaders().getListenPort() << "\n";
}

void    KeventHandler::disconnectClient(int client_fd)
{
    std::cout << "client disconnected: " << client_fd << std::endl;
    close(client_fd);
    fd_manager_.erase(client_fd);
    fd_content_.erase(client_fd);
}

void KeventHandler::changeEvents(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

void KeventHandler::setServerSocket(struct sockaddr_in *server_addr, Server server)
{
    memset(&(*server_addr), 0, sizeof(*server_addr));
    (*server_addr).sin_family = AF_INET;
    (*server_addr).sin_addr.s_addr = htonl(INADDR_ANY);
    (*server_addr).sin_port = htons(server.getListenPort());
}

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
        if (listen_socket_fd == -1)
            throw(std::runtime_error("socket() error\n"));

        if (bind(listen_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
            throw(std::runtime_error("bind() error\n"));

        if (listen(listen_socket_fd, 50) == -1)
            throw(std::runtime_error("listen() error\n"));
        fcntl(listen_socket_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

        server_sockets_.push_back(listen_socket_fd);
        changeEvents(change_list_, listen_socket_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);

        // using debug
        std::cout << "listen_socket_fd : " << listen_socket_fd << ", webserv on" << std::endl;
    }
}

void KeventHandler::initKqueue()
{
    kq_ = kqueue();
    if (kq_ == -1)
        throw(std::runtime_error("kqueue() error\n"));
}

bool    KeventHandler::createClientSocket(struct kevent* curr_event)
{
    bool result = false;

    for(size_t i = 0; i < server_sockets_.size(); i++)
    {
        if (curr_event->ident == server_sockets_[i])
        {
            int client_socket = accept(server_sockets_[i], NULL, NULL);
            if (client_socket == -1)
                throw(std::runtime_error("accept() error\n"));
            std::cout << "accept new client: " << client_socket << std::endl;
            fcntl(client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

            /* add event for client socket - add read & write event */
            EventRecorder st;
            fd_manager_[client_socket] = st;
            changeEvents(change_list_, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[client_socket]);
            changeEvents(change_list_, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[client_socket]);
            fd_content_[client_socket];
            result = true;
        }
    }
    return (result);
}

int KeventHandler::getServerIndex(Request req)
{
    for (size_t i = 0; i < http_.getServer().size(); i++)
    {
        if (req.getHeaders().getListenPort() == http_.getServer()[i].getListenPort() &&
            req.getHeaders().getHost() == http_.getServer()[i].getServerName())
            return (i);
    }
    return (0);
}

int KeventHandler::compareLocation(std::vector<std::string> t, std::vector<std::string> loc)
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

int KeventHandler::getLocationIndex(std::vector<std::string> request_target, Server &server, size_t *res_same_path_cnt)
{
    int index = 0;
    size_t same_path_cnt;

    for (size_t i = 1; i < server.getLocation().size(); i++)
    {
        same_path_cnt = compareLocation(request_target, server.getLocation()[i].getUrlPostfix());
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
    {
        return (0);
    }
    return (index);
}

std::string KeventHandler::makeDirList(std::string file_path)
{
    DIR* dir = opendir(file_path.c_str());
    if (!dir) {
        // 디렉토리 열기 실패
        throw (std::runtime_error("Error: Unable to open directory."));
    }

    std::string result;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::string name = entry->d_name;
        if (name != "." && name != "..") {
            result += name;
            result += "\n";
        }
    }
    closedir(dir);
    return (result);
}

bool KeventHandler::checkAccessMethod(std::string method, Location location)
{
    bool exist = false;

    for (size_t i = 0; i < location.getAccessMethod().size(); i++)
    {
        if (location.getAccessMethod()[i] == method)
            exist = true;
    }
    return (exist);
}

void KeventHandler::createResponseAutoindex(int curr_event_fd, std::string file_path)
{
    std::string version = fd_manager_[curr_event_fd].getRequest().getRequestLine().getVersion();

    fd_manager_[curr_event_fd].getResponse().getStatusLine().setVersion(version);
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("200");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("OK");
    fd_manager_[curr_event_fd].getResponse().getHeaders().setServer("default");
    fd_manager_[curr_event_fd].getResponse().getHeaders().setKeepAlive("timeout=100");

    time_t rawTime;
    time(&rawTime);

    struct tm *timeInfo;
    timeInfo = gmtime(&rawTime);

    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
    fd_manager_[curr_event_fd].getResponse().getHeaders().setDate(buffer);

    fd_manager_[curr_event_fd].getResponse().getHeaders().setContentType("text/html");

    fd_manager_[curr_event_fd].getResponse().getHeaders().setLastModified("default");
    fd_manager_[curr_event_fd].getResponse().getHeaders().setTransferEncoding("default");
    fd_manager_[curr_event_fd].getResponse().getHeaders().setConnection("keep-alive");
    fd_manager_[curr_event_fd].getResponse().getHeaders().setContentEncoding("default");


    std::string buf = makeDirList(file_path);
    fd_manager_[curr_event_fd].getResponse().getHeaders().setContentLength(std::to_string(buf.size()));
    fd_manager_[curr_event_fd].getResponse().setBody(buf);

    std::string res_tmp;
    res_tmp = "";
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setVersion("HTTP/1.1");
    res_tmp += (fd_manager_[curr_event_fd].getResponse().getStatusLine().getVersion() + " ");
    res_tmp += (fd_manager_[curr_event_fd].getResponse().getStatusLine().getStatusCode() + " ");
    res_tmp += (fd_manager_[curr_event_fd].getResponse().getStatusLine().getStatusText() + "\r\n");
    res_tmp += ("Date: " + fd_manager_[curr_event_fd].getResponse().getHeaders().getDate() + "\n");
    res_tmp += ("Content-Type: " + fd_manager_[curr_event_fd].getResponse().getHeaders().getContentType() + "\n");
    res_tmp += ("Content-Length: " + fd_manager_[curr_event_fd].getResponse().getHeaders().getContentLength() + "\n");
    res_tmp += ("Connection: " + fd_manager_[curr_event_fd].getResponse().getHeaders().getConnection() + "\n");
    res_tmp += ("Keep-Alive: " + fd_manager_[curr_event_fd].getResponse().getHeaders().getKeepAlive() + "\r\n");
    res_tmp += "\r\n" + charVectorToString(fd_manager_[curr_event_fd].getResponse().getBody());
    fd_content_[curr_event_fd] = stringToCharVector(res_tmp);

    fd_manager_[curr_event_fd].setEventWriteRes(1);
    changeEvents(change_list_, curr_event_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[curr_event_fd]);
}

// ========================================= method_utils =========================================

std::string KeventHandler::createFilePath(Server &server, Request &req, int loc_idx, size_t size)
{
    std::string file_path;

    file_path = server.getLocationBlock(loc_idx).getRoot();
    for (size_t i = 0; i < req.getRequestLine().getRequestTarget().size(); i++)
    {
        if (i < size)
            continue ;
        file_path += "/";
        file_path += req.getRequestLine().getRequestTarget()[i];
    }
    return (file_path);
}

void KeventHandler::setReadFileEvent(int curr_event_fd, int file_fd)
{
    fd_content_[file_fd];
    EventRecorder event_recorder(curr_event_fd);
    event_recorder.setEventReadFile(1);
    fd_manager_[file_fd] = event_recorder;        // 파일에 대한 상태값 표시
    fd_manager_[curr_event_fd].setEventWriteRes(-1);
    changeEvents(change_list_, file_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[file_fd]);
}

// void KeventHandler::setWriteFileEvent()
// {

// }

// ========================================= method_error =========================================

void KeventHandler::notAllowedMethod405(int curr_event_fd)
{
    int fd;

    fd = open ("./var/www/error/error_405.html", O_RDONLY);
    if (fd < 0)  // fd 에러처리해야함.
        throw (std::runtime_error("405 OPEN ERROR"));
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("405");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("Method Not Allowed");

    setReadFileEvent(curr_event_fd, fd);
}

void KeventHandler::notFound404(int curr_event_fd)
{
    int fd;

    fd = open("./var/www/error/error_404.html", O_RDONLY);
    if (fd < 0)
        throw (std::runtime_error("404 OPEN ERROR"));
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusCode("404");
    fd_manager_[curr_event_fd].getResponse().getStatusLine().setStatusText("Not Found");

    setReadFileEvent(curr_event_fd, fd);
}

// ========================================= post_utils =========================================

void KeventHandler::createFileForPost(int curr_event_fd, std::string file_path)
{
    int fd;

    fd = open(file_path.c_str(), O_RDWR | O_CREAT);
    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    fd_content_[fd] = stringToCharVector(fd_manager_[curr_event_fd].getRequest().getBody());    // fd_content[fd]에 request 받아온 body 넣어주기

    // setWriteFileEvent -> 따로 빼보기
    EventRecorder event_recorder(curr_event_fd);
    event_recorder.setEventWriteFile(1);
    fd_manager_[fd] = event_recorder;
    changeEvents(change_list_, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[fd]);
    fd_manager_[curr_event_fd].setEventWriteRes(-1);
    changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    // changeEvents(change_list_, curr_event_fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
}

// ========================================= post_method =========================================

void KeventHandler::methodPostHandler(Server &server, Request &req, int curr_event_fd)
{
    size_t  size;
    int     loc_idx;
    bool    is_allow_method;

    size = 0;
    loc_idx = getLocationIndex(req.getRequestLine().getRequestTarget(), server, &size);
    is_allow_method = checkAccessMethod(req.getRequestLine().getMethod(), server.getLocationBlock(loc_idx));
    if (is_allow_method == true)
    {
        std::string file_path;

        file_path = createFilePath(server, req, loc_idx, size);
        createFileForPost(curr_event_fd, file_path);
    }
    else
        notAllowedMethod405(curr_event_fd);
}

// ========================================= get_utils =========================================

bool isFaviconReq(Request &req)
{
    std::vector<std::string> request_target;

    request_target = req.getRequestLine().getRequestTarget();
    if (request_target.size() != 0 && request_target[0] == "favicon.ico")
        return (true);
    return (false);
}

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

int autoIndexStatus(Server &server, int loc_idx)
{
    if (server.getLocationBlock(loc_idx).getAutoIndex() == true)
        return (ON);
    return (OFF);
}

void KeventHandler::addFileName_getFileFd(std::string file_path, Server &server, int loc_idx, int curr_event_fd)
{
    int fd;

    (void)server;
    (void)loc_idx;
    // file_path += "/" + server.getLocationBlock(loc_idx).getIndex();
    fd = open (file_path.c_str(), O_RDONLY);
    if (fd < 0)
        notFound404(curr_event_fd);
    else
    {
        fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        setReadFileEvent(curr_event_fd, fd);
    }
}

// ========================================= get_method =========================================

void KeventHandler::methodGetHandler(Server &server, Request &req, int curr_event_fd)    // autoIndexFlag 필요없음
{
    int         loc_idx;
    size_t      size;
    std::string file_path;
    bool        is_allow_method = false;
    int         file_type;

    if (isFaviconReq(req))
        getFaviconFile(curr_event_fd);
    else                                  // isGetReq
    {
        size = 0;
        loc_idx = getLocationIndex(req.getRequestLine().getRequestTarget(), server, &size);
        is_allow_method = checkAccessMethod(req.getRequestLine().getMethod(), server.getLocationBlock(loc_idx));
        if (is_allow_method == true)
        {
            file_path = createFilePath(server, req, loc_idx, size);
            file_type = isFileOrDirectory(file_path.c_str());

            if (file_type == IS_DIR && autoIndexStatus(server, loc_idx) == OFF)
            {
                file_path += "/" + server.getLocationBlock(loc_idx).getIndex();
                addFileName_getFileFd(file_path, server, loc_idx, curr_event_fd);
            }
            else if (file_type == IS_DIR && autoIndexStatus(server, loc_idx) == ON)
                createResponseAutoindex(curr_event_fd, file_path);
            else if (file_type == IS_FILE)
                addFileName_getFileFd(file_path, server, loc_idx, curr_event_fd);
            else if (file_type == FILE_NOT_FOUND)
                notFound404(curr_event_fd);
        }
        else
            notAllowedMethod405(curr_event_fd);
    }
    changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
}

void    KeventHandler::createRequest(struct kevent* curr_event)                            // ==============create reques================================================
{
    std::string url;
    Request req;
    std::string cur_method = fd_manager_[curr_event->ident].getRequest().getRequestLine().getMethod();

    if ((cur_method == "GET" || cur_method == "POST" || cur_method == "DELETE" || cur_method == "PUT") == false)
    {
        notAllowedMethod405(curr_event->ident);
        changeEvents(change_list_, curr_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        return ;
    }

    req = fd_manager_[curr_event->ident].getRequest();
    int index = getServerIndex(req);

    if (req.getRequestLine().getMethod() == "GET")
    {
        methodGetHandler(http_.getServer()[index], req, curr_event->ident);
    }
    else if (req.getRequestLine().getMethod() == "POST")
    {
        methodPostHandler(http_.getServer()[index], req, curr_event->ident);
    }
}

void KeventHandler::createResponse(struct kevent* curr_event)
{
    int parent_fd = fd_manager_[curr_event->ident].getParentClientFd();
    std::string version = fd_manager_[parent_fd].getRequest().getRequestLine().getVersion();

    fd_manager_[parent_fd].getResponse().getStatusLine().setVersion(version);
    // fd_manager_[parent_fd].getResponse().getStatusLine().setStatusCode("200");
    // fd_manager_[parent_fd].getResponse().getStatusLine().setStatusText("OK");
    fd_manager_[parent_fd].getResponse().getHeaders().setServer("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setKeepAlive("timeout=100");

    time_t rawTime;
    time(&rawTime);

    struct tm *timeInfo;
    timeInfo = gmtime(&rawTime);

    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);

    fd_manager_[parent_fd].getResponse().getHeaders().setDate(buffer);

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size() != 0 && fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget()[0] == "/favicon.ico")
        fd_manager_[parent_fd].getResponse().getHeaders().setContentType("image/x-icon");
    else
        fd_manager_[parent_fd].getResponse().getHeaders().setContentType("text/html");


    fd_manager_[parent_fd].getResponse().getHeaders().setLastModified("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setTransferEncoding("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setConnection("keep-alive");
    fd_manager_[parent_fd].getResponse().getHeaders().setContentEncoding("default");

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size() != 0 && fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget()[0] == "/favicon.ico")
        fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(1150));
    else
        fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(fd_content_[curr_event->ident].size()));

    std::string file_data(charVectorToString(fd_content_[curr_event->ident]));

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getMethod() == "HEAD")
        file_data = "";
    fd_manager_[parent_fd].getResponse().setBody(file_data);

    std::string res_tmp;
    res_tmp = "";
    fd_manager_[parent_fd].getResponse().getStatusLine().setVersion("HTTP/1.1");
    res_tmp += (fd_manager_[parent_fd].getResponse().getStatusLine().getVersion() + " ");
    res_tmp += (fd_manager_[parent_fd].getResponse().getStatusLine().getStatusCode() + " ");
    res_tmp += (fd_manager_[parent_fd].getResponse().getStatusLine().getStatusText() + "\r\n");
    res_tmp += ("Date: " + fd_manager_[parent_fd].getResponse().getHeaders().getDate() + "\n");
    res_tmp += ("Content-Type: " + fd_manager_[parent_fd].getResponse().getHeaders().getContentType() + "\n");
    res_tmp += ("Content-Length: " + fd_manager_[parent_fd].getResponse().getHeaders().getContentLength() + "\n");
    res_tmp += ("Connection: " + fd_manager_[parent_fd].getResponse().getHeaders().getConnection() + "\n");
    res_tmp += ("Keep-Alive: " + fd_manager_[parent_fd].getResponse().getHeaders().getKeepAlive() + "\r\n");
    res_tmp += "\r\n" + charVectorToString(fd_manager_[parent_fd].getResponse().getBody());
    fd_content_[parent_fd] = stringToCharVector(res_tmp);

    fd_manager_[parent_fd].setEventWriteRes(1);
    close(curr_event->ident);
    fd_content_.erase(curr_event->ident);
    fd_manager_.erase(curr_event->ident);
    changeEvents(change_list_, parent_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[parent_fd]);
}

void    KeventHandler::createFile(struct kevent* curr_event)
{
    int parent_fd = fd_manager_[curr_event->ident].getParentClientFd();

    int n = write(curr_event->ident, fd_manager_[parent_fd].getRequest().getBody().c_str(), fd_manager_[parent_fd].getRequest().getHeaders().getContentLength());
    if (n == -1)
    {
        std::cerr << "file write error!" << std::endl;
        // disconnectClient(curr_event->ident);
        fd_manager_[curr_event->ident].setFdError(1);
    }

    fd_manager_[parent_fd].getResponse().getStatusLine().setStatusCode("200");
    fd_manager_[parent_fd].getResponse().getStatusLine().setStatusText("OK");
    createResponse(curr_event);
}


void    KeventHandler::sendResponse(struct kevent* curr_event)
{
    int n = write(curr_event->ident, &(*fd_content_[curr_event->ident].begin()), fd_content_[curr_event->ident].size());

    // std::cout << "res: \n" << charVectorToString(fd_content_[curr_event->ident]) << "\n";
    std::cout << "status code: " << fd_manager_[curr_event->ident].getResponse().getStatusLine().getStatusCode() << "\n";

    if (n == -1)
    {
        std::cerr << "client write error!" << std::endl;
        disconnectClient(curr_event->ident);
        fd_manager_[curr_event->ident].setFdError(1);
    }
    else
    {
        fd_content_[curr_event->ident].clear();
        fd_manager_[curr_event->ident].setEventWriteRes(-1);
        changeEvents(change_list_, curr_event->ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        changeEvents(change_list_, curr_event->ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[curr_event->ident]);
        EventRecorder n;
        fd_manager_[curr_event->ident] = n;
    }
}

void    KeventHandler::parsingReqStartLineAndHeaders(struct kevent* curr_event)  
{
    Request req;

    std::string buf;
    std::istringstream streamLine(charVectorToString(fd_content_[curr_event->ident]));

    // std::cout << "request : [" << charVectorToString(fd_content_[curr_event->ident]) << "]" << std::endl;

    std::getline(streamLine, buf, ' ');
    // if ((buf == "GET" || buf == "POST" || buf == "DELETE" || buf == "PUT") == false)
    // {
    //     notAllowedMethod405(curr_event->ident);
    //     changeEvents(change_list_, curr_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    //     return ;
    // }
    req.getRequestLine().setMethod(buf);
    std::getline(streamLine, buf, ' ');
    req.getRequestLine().setRequestTarget(buf);
    std::getline(streamLine, buf);
    req.getRequestLine().setVersion(buf);

    // std::cout << "req" << std::endl;
    while (std::getline(streamLine, buf))   //Request Header
    {
        if (buf == "\r")  // \r이 없는 경우 처리해야할지 고민하기
        {
            // std::cout << "---------------" << std::endl;
            std::getline(streamLine, buf);
            req.setBody(buf);
        }
        else
        {
            std::istringstream streamLine2(buf);

            std::getline(streamLine2, buf, ' ');
            if (buf == "Host:") {
                std::getline(streamLine2, buf);
                // std::cout << "buf: " << buf << std::endl;
                req.getHeaders().setFullPath(buf);
            }
            else if (buf == "Accept:")
            {
                std::getline(streamLine2, buf);
                // std::cout << "Accept: " << buf << std::endl;
                req.getHeaders().setAccept(buf);
            }
            else if (buf == "Accept-Encoding:")
            {
                std::getline(streamLine2, buf);
                // std::cout << "Accept-Encoding: " << buf << std::endl;
                req.getHeaders().setAcceptEncoding(buf);
            }
            else if (buf == "Accept-Language:")
            {
                std::getline(streamLine2, buf);
                // std::cout << "Accept-Language: " << buf << std::endl;
                req.getHeaders().setAcceptLanguage(buf);
            }
            else if (buf == "Connection:")
            {
                std::getline(streamLine2, buf);
                req.getHeaders().setConnection(buf);
            }
            else if (buf == "Upgrade-Insecure-Requests:")
            {
                std::getline(streamLine2, buf);
                req.getHeaders().setUpgradeInsecureRequests(buf);
            }
            else if (buf == "User-Agent:")
            {
                std::getline(streamLine2, buf);
                req.getHeaders().setUserAgent(buf);
            }
            else if (buf == "Content-Length:")
            {
                std::getline(streamLine2, buf);
                // std::cout << "Content-Length: " << buf << std::endl;
                int length = std::atoi(&buf[0]);
                req.getHeaders().setContentLength(length);
                fd_manager_[curr_event->ident].setReadType(READ_CONTENT_LENGTH_BODY);
            }
            else if (buf == "Transfer-Encoding:")
            {
                std::getline(streamLine2, buf);
                // std::cout << "Transfer-Encoding: " << buf << std::endl;
                req.getHeaders().setTransferEncoding(buf);
                fd_manager_[curr_event->ident].setReadType(READ_CHUNKED_BODY);
            }
        }
    }
    if (fd_manager_[curr_event->ident].getReadType() != READ_CONTENT_LENGTH_BODY && fd_manager_[curr_event->ident].getReadType() != READ_CHUNKED_BODY)
        fd_manager_[curr_event->ident].setReadType(NO_EXIST_BODY);
    fd_manager_[curr_event->ident].setRequest(req);
}

int KeventHandler::addSegmentReqAndReadMode(struct kevent* curr_event, char buf[], int n)
{
    int i = 0;

    if (fd_manager_[curr_event->ident].getReadType() == READ_HEADER)
    {
        // for (int i = 0; i < n; i++)
        for (; i < n; i++)
        {
            // std::cout << "EOF FLAG: " << fd_manager_[curr_event->ident].getHeaderEof() << std::endl;
            // if (fd_manager_[curr_event->ident].getReadType() != READ_HEADER)
            // {

            //     fd_content_[curr_event->ident].push_back(buf[i]);
            //     continue;
            // }
            if (buf[i] == '\r' && fd_manager_[curr_event->ident].getHeaderEof() == 0)
                fd_manager_[curr_event->ident].setHeaderEof(1);
            else if (buf[i] == '\n' && fd_manager_[curr_event->ident].getHeaderEof() == 1)
                fd_manager_[curr_event->ident].setHeaderEof(2);
            else if (buf[i] == '\r' && fd_manager_[curr_event->ident].getHeaderEof() == 2)
                fd_manager_[curr_event->ident].setHeaderEof(3);
            else if (buf[i] == '\n' && fd_manager_[curr_event->ident].getHeaderEof() == 3)
                fd_manager_[curr_event->ident].setHeaderEof(4);
            else
                fd_manager_[curr_event->ident].setHeaderEof(0);

            fd_content_[curr_event->ident].push_back(buf[i]);
            if (fd_manager_[curr_event->ident].getHeaderEof() == 4) // 헤더 끝 읽음
            {
                std::cout << "----------------hearder eof----------------" << std::endl;
                parsingReqStartLineAndHeaders(curr_event);
                fd_content_[curr_event->ident].clear();
                fd_manager_[curr_event->ident].setHeaderEof(0);
                break ;
            }
        }
    }

    if (fd_manager_[curr_event->ident].getReadType() == NO_EXIST_BODY)
        return (READ_FINISH_REQUEST);

    if (fd_manager_[curr_event->ident].getReadType() == READ_CONTENT_LENGTH_BODY)
    {
        if (fd_manager_[curr_event->ident].getRequest().getHeaders().getContentLength() == 0)
            return (READ_FINISH_REQUEST);
        for (; i < n; i++)
        {
            if (fd_manager_[curr_event->ident].getContentCurrentReadLength() < fd_manager_[curr_event->ident].getRequest().getHeaders().getContentLength())
            {
                fd_content_[curr_event->ident].push_back(buf[i]);
                fd_manager_[curr_event->ident].incContentCurrentReadLength();
                
                if (fd_manager_[curr_event->ident].getContentCurrentReadLength() == fd_manager_[curr_event->ident].getRequest().getHeaders().getContentLength())
                {
                    std::string body;

                    body = charVectorToString(fd_content_[curr_event->ident]);
                    fd_manager_[curr_event->ident].getRequest().setBody(body);
                    fd_manager_[curr_event->ident].setContentCurrentReadLength(0);
                    return (READ_FINISH_REQUEST);
                }
            }
        }
    }
    else if (fd_manager_[curr_event->ident].getReadType() == READ_CHUNKED_BODY)
    {
        for (; i < n; i++)
        {
            if (buf[i] == '\r' && fd_manager_[curr_event->ident].getChunkedCrLf() == 0)
                fd_manager_[curr_event->ident].setChunkedCrLf(1);
            else if (buf[i] == '\n' && fd_manager_[curr_event->ident].getChunkedCrLf() == 1)
                fd_manager_[curr_event->ident].setChunkedCrLf(2);
            else
            {
                fd_manager_[curr_event->ident].setChunkedCrLf(0);
                // fd_content_[curr_event->ident].push_back(buf[i]);
            }

            //chunked_length의 정보를 임시로 저장할 vector_char 활용하기
            if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_LENGTH)
            {
                if (fd_manager_[curr_event->ident].getChunkedCrLf() == 0)
                    fd_manager_[curr_event->ident].pushbackChunkedLengthTemp(buf[i]);
                if (fd_manager_[curr_event->ident].getChunkedCrLf() == 2)
                {
                    fd_manager_[curr_event->ident].pushbackChunkedLengthTemp('\0');
                    std::string chunked_length_str = charVectorToString(fd_content_[curr_event->ident]);
                    fd_manager_[curr_event->ident].setChunkedTotalReadLength(hexToDecimal(chunked_length_str.c_str(), chunked_length_str.size()));
                    fd_manager_[curr_event->ident].clearChunkedLengthTemp();
                    fd_manager_[curr_event->ident].setChunkedDataType(CHUNKED_DATA);
                    fd_manager_[curr_event->ident].setChunkedCrLf(0);
                }
            }
            else if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_DATA)
            {
                if (fd_manager_[curr_event->ident].getChunkedTotalReadLength() == 0 && fd_manager_[curr_event->ident].getChunkedCrLf() == 2)
                {
                    std::string body;

                    body = charVectorToString(fd_content_[curr_event->ident]);
                    fd_manager_[curr_event->ident].getRequest().setBody(body);
                    return (READ_FINISH_REQUEST);
                }
                if (fd_manager_[curr_event->ident].getContentCurrentReadLength() < fd_manager_[curr_event->ident].getChunkedTotalReadLength())
                {
                    fd_content_[curr_event->ident].push_back(buf[i]);
                    fd_manager_[curr_event->ident].incContentCurrentReadLength();
                    
                    if (fd_manager_[curr_event->ident].getContentCurrentReadLength() == fd_manager_[curr_event->ident].getChunkedTotalReadLength())
                    {
                        fd_manager_[curr_event->ident].setContentCurrentReadLength(0);
                        fd_manager_[curr_event->ident].setChunkedDataType(CHUNKED_LENGTH);
                    }
                }
            }
        }
    }
    else // 바디가 없는 경우
    { 
        
    }
    return (IDLE);
    // 플래그 이름 : f
    // cr lf cr lf
    // crlf나올때가지 받는 거 -> f: 5
    // 헤더 파싱 해주고 -> content-lengthㄹ가 있으면 f: 6, chuncked 이면 f: 7

    // 만약 위에서 crlfcrlf 나왔으면 -> 헤더 파싱하는 ENUM 반환 (밑에서 case 문 추가)
    // 
}

int KeventHandler::readFdFlag(struct kevent* curr_event, char *buf, int *n)
{
    if (fd_content_.find(curr_event->ident) != fd_content_.end())
    {
        memset(buf, 0, BUFFER_SIZE);
        *n = read(curr_event->ident, buf, BUFFER_SIZE - 1);
        if (*n > 0)
        {
            if (fd_manager_[curr_event->ident].getEventReadFile() == 1)
                // std::cout << "file : [" << buf << "]" << std::endl;
                ;
            // else
            // {
            //     write (1, "req start : [[[[[", sizeof("req start : [[[[["));
            //     reqPrint(buf, *n);
            //     write (1, "]]]]]\n\n", sizeof("]]]]]\n\n"));
            // }
        }
        if (*n < 0 && (curr_event->flags & EV_EOF))
            return (CLOSE_CONNECTION);
        if (*n < 0)
            return (READ_ERROR);
        buf[*n] = '\0';
        // std::cout << "data size: " << curr_event->data << " n: " << *n << "\n";
        if (curr_event->data <= *n)
        {
            // std::cout << "getEventReadFile : " << fd_manager_[curr_event->ident].getEventReadFile() << "\n";
            if (fd_manager_[curr_event->ident].getEventReadFile() == -1)
            {
                return (addSegmentReqAndReadMode(curr_event, buf, *n));
                // if (fd_content_[curr_event->ident].size() != 0)
                //     return (READ_FINISH_REQUEST);
            }
            else     // 파일인 경우
            {
                // std::cout << "addContent func\n";
                addContent(curr_event, buf, *n);
                return (READ_FINISH_FILE);
            }
        }
        else
            return (READ_CONTINUE);
    }
    if (curr_event->flags & EV_EOF)
        return (CLOSE_CONNECTION);
    return (-1);
}

bool KeventHandler::isSocket(struct kevent* curr_event)
{
     for(size_t i = 0; i < server_sockets_.size(); i++)
    {
        if (curr_event->ident == server_sockets_[i])
            return (true);
    }
    return (false);
}

int  KeventHandler::writeFdFlag(struct kevent* curr_event)
{
    std::map<int, std::vector<char> >::iterator it = fd_content_.find(curr_event->ident);
    if (it != fd_content_.end())
    {
        if (fd_content_[curr_event->ident].size() != 0 && fd_manager_[curr_event->ident].getEventWriteRes() == 1)
        {
            // std::cout << "send response!" << std::endl;
            // std::cout << charVectorToString(fd_content_[curr_event->ident]) << std::endl;
            return (SEND_RESPONSE);
        }
        else if (fd_manager_[curr_event->ident].getEventWriteFile() == 1)
        // else
            return (EDIT_FILE);
        else
            return (IDLE);
    }
    return (-1);
}

int  KeventHandler::getEventFlag(struct kevent* curr_event, char *buf, int *n)
{
    if (curr_event->flags & EV_ERROR)
        return (ERROR);
    else if (curr_event->filter == EVFILT_READ)
    {
        if (isSocket(curr_event))
            return (IS_SERVER_SOCKET);
        return (readFdFlag(curr_event, buf, n));
    }
    else if (curr_event->filter == EVFILT_WRITE)
        return (writeFdFlag(curr_event));
    return (-1);
}

void KeventHandler::socketError(struct kevent*  curr_event)
{
    for(size_t i = 0; i < server_sockets_.size(); i++)
    {
        if (curr_event->ident == server_sockets_[i])
            throw(std::runtime_error("server socket error"));
        else
        {
            throw(std::runtime_error("client socket error"));
            disconnectClient(curr_event->ident);
        }
    }
}

void KeventHandler::clientReadError(struct kevent* curr_event)
{
    std::cerr << "client read error!" <<std::endl;
    disconnectClient(curr_event->ident);
}

void KeventHandler::addContent(struct kevent* curr_event, char buf[], int n)
{
    fd_content_[curr_event->ident].insert(fd_content_[curr_event->ident].end(), buf, buf + n);
}

void KeventHandler::runServer(void)
{
    int             new_events;
    int             event_type;
    struct kevent*  curr_event;
    char            buf[BUFFER_SIZE];
    int             n;

    initKqueue();
    while (1)
    {
        new_events = kevent(kq_, &change_list_[0], change_list_.size(), event_list_, EVENT_LIST_SIZE, NULL);
        if (new_events == -1)
            throw(std::runtime_error("kevent() error\n"));

        change_list_.clear();
        for (int i = 0; i < new_events; ++i)
        {
            curr_event = &event_list_[i];
            event_type = getEventFlag(curr_event, buf, &n);

            switch(event_type)
            {
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
                    createRequest(curr_event);
                    break ;

                case READ_FINISH_FILE :
                    createResponse(curr_event);
                    break ;

                case READ_CONTINUE :
                    addContent(curr_event, buf, n);
                    break ;

                case SEND_RESPONSE :
                    sendResponse(curr_event);
                    break ;

                case EDIT_FILE :
                    createFile(curr_event);
                    break ;

                case CLOSE_CONNECTION :
                    disconnectClient(curr_event->ident);
                    break ;

                case IDLE :
                    // std::cout << "IDLE" << std::endl;
                    break ;

                default :
                {
                    throw(std::runtime_error("event exception error\n"));
                }
            }
        }
    }
}
