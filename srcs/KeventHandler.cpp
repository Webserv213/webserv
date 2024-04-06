#include "KeventHandler.hpp"
#include <dirent.h>
#include <sys/stat.h>

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
        EventRecorder st;
        fd_manager_[listen_socket_fd] = st;
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
            if (client_socket < 0)
                throw(std::runtime_error("accept() error\n"));
            std::cout << "accept new client: " << client_socket << std::endl;
            fcntl(client_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);

            /* add event for client socket - add read & write event */
            EventRecorder st;
            fd_manager_[client_socket] = st;
            changeEvents(change_list_, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[client_socket]);
            // changeEvents(change_list_, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[client_socket]);
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


bool KeventHandler::checkCgi(Request req, Location loc, std::string extension)
{
    if (loc.getUrlPostfix().size() == 1 && loc.getUrlPostfix()[0] == extension && req.getRequestLine().getMethod() == "POST")
        return (true);
    return (false);
}

int KeventHandler::getLocationIndex(Request req, Server &server, size_t *res_same_path_cnt)
{
    int index = 0;
    size_t same_path_cnt;
    std::vector<std::string> request_target;

    std::string cgi_file_extension = "";
    size_t size = 0;
    request_target = req.getRequestLine().getRequestTarget();

    // 확장자 검사
    if (request_target.size() > 0 && request_target[request_target.size() - 1].rfind('.') != std::string::npos)
    {
        size = request_target[request_target.size() - 1].rfind('.');
        cgi_file_extension = request_target[request_target.size() - 1].substr(size);
    }

    for (size_t i = 1; i < server.getLocation().size(); i++)
    {
        same_path_cnt = compareLocation(request_target, server.getLocation()[i].getUrlPostfix());
        if (checkCgi(req, server.getLocation()[i], cgi_file_extension))   // fd_manager의 리퀘스트의 메소드 벡터 확인
        {
            *res_same_path_cnt = 1;     // 임시적인거고
            return (i);
        }

        if (same_path_cnt == server.getLocation()[i].getUrlPostfix().size())
        {
            std::cout << "res_same_path_cnt1 : " << res_same_path_cnt << std::endl;
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
    fd_manager_[curr_event_fd].getResponse().addBody(buf);

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
        // req.getRequestLine().getRequestTarget()[i] dir 검사하는 로직 (근데 마지막은 파일이어야함) -> 아니면 404 에러 던지기
        file_path += req.getRequestLine().getRequestTarget()[i];
    }
    std::cout << "createFilePath\n";
    return (file_path);
}

void KeventHandler::setReadFileEvent(int curr_event_fd, int file_fd)
{
    std::cout << "set read file event\n";
    fd_content_[file_fd];
    EventRecorder event_recorder(curr_event_fd);
    event_recorder.setEventReadFile(1);
    fd_manager_[file_fd] = event_recorder;        // 파일에 대한 상태값 표시
    fd_manager_[curr_event_fd].setEventWriteRes(-1);
    changeEvents(change_list_, file_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[file_fd]);
}

// ========================================= method_error =========================================

void KeventHandler::notAllowedMethod405(int curr_event_fd)
{
    int fd;

    fd = open ("./var/www/error/error_405.html", O_RDONLY);
    if (fd < 0)   // fd 에러 처리하기
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

    std::cout << "file_path :" << file_path << std::endl;
    fd = open(file_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        std::runtime_error("file open [post]");

    std::cout << "file fd: " << fd << "\n";

    fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    fd_content_[fd] = stringToCharVector(fd_manager_[curr_event_fd].getRequest().getBody());    // fd_content[fd]에 request 받아온 body 넣어주기

    std::cout << "create file post buf: " << charVectorToString(fd_content_[fd]) << "\n";

    // setWriteFileEvent -> 따로 빼보기
    EventRecorder event_recorder(curr_event_fd);
    event_recorder.setEventWriteFile(1);
    fd_manager_[fd] = event_recorder;
    if (fd_manager_[curr_event_fd].getCgiStatus() == DONE_CGI)
    {
        std::string str = charVectorToString(fd_content_[fd]);
        fd_manager_[fd].getRequest().addBody(str);
    }
    changeEvents(change_list_, fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[fd]);
    fd_manager_[curr_event_fd].setEventWriteRes(-1);
    if (fd_manager_[curr_event_fd].getCgiStatus() != DONE_CGI)  // cgi아닌 경우만 delete 안함
        changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
}

// ========================================= post_method =========================================

void KeventHandler::methodPostHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size)
{
    bool    is_allow_method;

    std::cout << "5 [post in]\n";
    is_allow_method = checkAccessMethod(req.getRequestLine().getMethod(), server.getLocationBlock(loc_idx));
    if (is_allow_method == true)
    {
        std::cout << "5.5\n";
        std::string file_path;

        file_path = createFilePath(server, req, loc_idx, size);
        createFileForPost(curr_event_fd, file_path);
    }
    else
    {
        std::cout << "6\n";
        notAllowedMethod405(curr_event_fd);
    }
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

void KeventHandler::openFile(std::string file_path, int curr_event_fd)
{
    int fd;
    struct stat st;

    std::cout << "file_path: " << file_path << std::endl;
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

// ========================================= get_method =========================================

void KeventHandler::methodGetHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size)    // autoIndexFlag 필요없음
{
    std::string file_path;
    bool        is_allow_method = false;
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

            if (file_type == IS_DIR && autoIndexStatus(server, loc_idx) == OFF)
            {
                file_path += "/" + server.getLocationBlock(loc_idx).getIndex();
                openFile(file_path, curr_event_fd);
            }
            else if (file_type == IS_DIR && autoIndexStatus(server, loc_idx) == ON)
                createResponseAutoindex(curr_event_fd, file_path);
            else if (file_type == IS_FILE)
            {
                std::cout << "get: is file\n";
                openFile(file_path, curr_event_fd);
            }
            else if (file_type == FILE_NOT_FOUND)
                notFound404(curr_event_fd);
        }
        else
            notAllowedMethod405(curr_event_fd);
    }
    changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
}

// ========================================= delete_method =========================================

void    KeventHandler::methodDeleteHandler(Server &server, Request &req, int curr_event_fd, int loc_idx, size_t size)
{
    int     fd;
    bool    is_allow_method;

    is_allow_method = checkAccessMethod(req.getRequestLine().getMethod(), server.getLocationBlock(loc_idx));
    if (is_allow_method == true)
    {
        std::string file_path;

        file_path = createFilePath(server, req, loc_idx, size);
        // file_path 있는지 확인해서 있으면 지우고, 없으면 -> 404
        fd = open (file_path.c_str(), O_RDONLY);
        if (fd < 0)
            notFound404(curr_event_fd);
        else
            // 삭제
            ;
    }
    else
        notAllowedMethod405(curr_event_fd);
}

bool    KeventHandler::isCgiRequest(int cur_fd, int idx, int loc_idx)
{
    int fd[2];

    std::cout << "3.1\n";
    if (fd_manager_[cur_fd].getCgiStatus() != DONE_CGI && http_.getServer()[idx].getLocationBlock(loc_idx).getCgiPath() != "")
    {
        std::cout << "3.1.1\n";
        std::string clear_body = "";
        fd_manager_[cur_fd].setSendCgiBody(fd_manager_[cur_fd].getRequest().getBody());
        fd_manager_[cur_fd].getRequest().setBody(clear_body);
        pipe(fd);
        EventRecorder pipe_event_recorder(cur_fd);
        fd_manager_[fd[1]] = pipe_event_recorder;
        fd_manager_[fd[0]] = pipe_event_recorder;

        fd_manager_[cur_fd].setSendPipe(0, fd[0]);
        fd_manager_[cur_fd].setSendPipe(1, fd[1]);
        fd_manager_[fd[1]].setCgiStatus(WRITE_CGI);
        fd_manager_[cur_fd].setCgiPath(http_.getServer()[idx].getLocationBlock(loc_idx).getCgiPath());
        changeEvents(change_list_, fd_manager_[cur_fd].getSendPipe(1), EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, NULL);
        changeEvents(change_list_, cur_fd , EVFILT_READ, EV_DELETE, 0, 0, NULL);
        return (true);
    }
    std::cout << "3.2\n";
    return (false);
}

bool    KeventHandler::isRightMethod(Request &req, int cur_fd)
{
    std::string cur_method;

    cur_method = req.getRequestLine().getMethod();
    if ((cur_method == "GET" || cur_method == "POST" || cur_method == "DELETE") == false)
    {
        notAllowedMethod405(cur_fd);
        changeEvents(change_list_, cur_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        return (false);
    }
    return (true);
}

void    KeventHandler::executeMethod(int cur_fd)
{
    int idx;
    int loc_idx;
    size_t size = 0;
    Request req = fd_manager_[cur_fd].getRequest();

    std::cout << "1\n";
    idx = getServerIndex(req);
    std::cout << "2\n";
    loc_idx = getLocationIndex(req, http_.getServer()[idx], &size);
    std::cout << "3\n";

    if (isRightMethod(req, cur_fd) == false)
        return ;
    if (isCgiRequest(cur_fd, idx, loc_idx))
        return ;

    std::cout << "4\n";
    if (req.getRequestLine().getMethod() == "GET")
        methodGetHandler(http_.getServer()[idx], req, cur_fd, loc_idx, size);
    else if (req.getRequestLine().getMethod() == "POST")
        methodPostHandler(http_.getServer()[idx], req, cur_fd, loc_idx, size);
    else if (req.getRequestLine().getMethod() == "DELETE")
        methodDeleteHandler(http_.getServer()[idx], req, cur_fd, loc_idx, size);
}

void KeventHandler::createResponse(unsigned int cur_fd)
{
    int parent_fd = fd_manager_[cur_fd].getParentClientFd();
    std::string version = fd_manager_[parent_fd].getRequest().getRequestLine().getVersion();

    fd_manager_[parent_fd].getResponse().getStatusLine().setVersion(version);
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


    std::string file_data(charVectorToString(fd_content_[cur_fd]));

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getMethod() == "HEAD")
        file_data = "";
    fd_manager_[parent_fd].getResponse().addBody(file_data);

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget().size() != 0 && fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget()[0] == "/favicon.ico")
        fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(1150));
    else
        fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(fd_content_[cur_fd].size()));
        // fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(fd_manager_[parent_fd].getRequest().getBody().size()));

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
    close(cur_fd);    // close, delete event 이거 지워도 되나 확인해보기 (cgi인 경우만 지워야되는건지))
    // changeEvents(change_list_, cur_fd, EVFILT_WRITE, EV_DELETE, 0, 0, &fd_manager_[parent_fd]);
    fd_content_.erase(cur_fd);
    fd_manager_.erase(cur_fd);
    changeEvents(change_list_, parent_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[parent_fd]);
}

void    KeventHandler::createFile(struct kevent* curr_event)
{
    int parent_fd = fd_manager_[curr_event->ident].getParentClientFd();

    std::cout << "parent fd: " << parent_fd << "\n";
    // int n = write(curr_event->ident, fd_manager_[parent_fd].getRequest().getBody().c_str(), fd_manager_[parent_fd].getRequest().getHeaders().getContentLength());
    int n = write(curr_event->ident, fd_manager_[parent_fd].getRequest().getBody().c_str(), fd_manager_[parent_fd].getRequest().getBody().size());
    std::cout << "create file: " << fd_manager_[parent_fd].getRequest().getBody() << "\n";



    if (n < 0)
    {
        std::cerr << "file write error!" << std::endl;
        // disconnectClient(curr_event->ident);
        fd_manager_[curr_event->ident].setFdError(1);
    }

    fd_manager_[parent_fd].getResponse().getStatusLine().setStatusCode("200");
    fd_manager_[parent_fd].getResponse().getStatusLine().setStatusText("OK");
    createResponse(curr_event->ident);
}


// void    KeventHandler::sendResponse(struct kevent* curr_event)
void    KeventHandler::sendResponse(unsigned int curr_event_fd)
{
    int n = write(curr_event_fd, &(*fd_content_[curr_event_fd].begin()), fd_content_[curr_event_fd].size());

    if (n == -1)
    {
        std::cerr << "client write error!" << std::endl;
        disconnectClient(curr_event_fd);
        fd_manager_[curr_event_fd].setFdError(1);
    }
    else
    {
        fd_content_[curr_event_fd].clear();
        fd_manager_[curr_event_fd].setEventWriteRes(-1);
        changeEvents(change_list_, curr_event_fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
        changeEvents(change_list_, curr_event_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[curr_event_fd]);
        EventRecorder n;
        fd_manager_[curr_event_fd] = n;
    }
}

void    KeventHandler::parsingReqStartLineAndHeaders(struct kevent* curr_event)
{
    Request req;
    std::string buf;
    std::string key;
    std::string value;
    std::istringstream full_content(charVectorToString(fd_content_[curr_event->ident]));

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
        if (buf == "\r")  // \r이 없는 경우 처리해야할지 고민하기
        {
            std::getline(full_content, buf);
            req.addBody(buf);
        }
        else
        {
            std::istringstream one_line(buf);

            std::getline(one_line, key, ' ');
            std::getline(one_line, value);

            if (key == "Host:")
                req.getHeaders().setFullPath(value);
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
            else if (key == "Content-Length:")
            {
                int length = std::atoi(&value[0]);
                req.getHeaders().setContentLength(length);
                fd_manager_[curr_event->ident].setReadType(READ_CONTENT_LENGTH_BODY);
            }
            else if (key == "Transfer-Encoding:")
            {
                req.getHeaders().setTransferEncoding(value);
                fd_manager_[curr_event->ident].setReadType(READ_CHUNKED_BODY);
            }
        }
    }
    if (fd_manager_[curr_event->ident].getReadType() != READ_CONTENT_LENGTH_BODY
        && fd_manager_[curr_event->ident].getReadType() != READ_CHUNKED_BODY)
        fd_manager_[curr_event->ident].setReadType(NO_EXIST_BODY);
    fd_manager_[curr_event->ident].setRequest(req);
}

// 나누어져서 들어오는 Request 메시지를 조립하는 함수
int KeventHandler::readReqHeader(struct kevent* curr_event)
{
    size_t  i = fd_manager_[curr_event->ident].getFdContentIndex();

    for (; i < fd_content_[curr_event->ident].size(); i++)
    {
        if (fd_content_[curr_event->ident].at(i) == '\r' && fd_manager_[curr_event->ident].getHeaderEof() == 0)
            fd_manager_[curr_event->ident].setHeaderEof(1);
        else if (fd_content_[curr_event->ident].at(i) == '\n' && fd_manager_[curr_event->ident].getHeaderEof() == 1)
            fd_manager_[curr_event->ident].setHeaderEof(2);
        else if (fd_content_[curr_event->ident].at(i) == '\r' && fd_manager_[curr_event->ident].getHeaderEof() == 2)
            fd_manager_[curr_event->ident].setHeaderEof(3);
        else if (fd_content_[curr_event->ident].at(i) == '\n' && fd_manager_[curr_event->ident].getHeaderEof() == 3)
        {
            fd_manager_[curr_event->ident].setHeaderEof(4);
            i++;
            break ;
        }
        else
            fd_manager_[curr_event->ident].setHeaderEof(0);
    }
    fd_manager_[curr_event->ident].setFdContentIndex(i);
    if (fd_manager_[curr_event->ident].getHeaderEof() == 4)
    {
        std::cout << "----------------HEADER EOF----------------" << std::endl;
        std::vector<char>::iterator it = fd_content_[curr_event->ident].begin();
        parsingReqStartLineAndHeaders(curr_event);
        fd_content_[curr_event->ident].erase(it, it + i);
        fd_manager_[curr_event->ident].setFdContentIndex(0);
        fd_manager_[curr_event->ident].setHeaderEof(0);
    }
    else
        return (IDLE);
    return (fd_manager_[curr_event->ident].getReadType());
}

// Content-length 타입의 Body를 읽는 함수
int KeventHandler::readContentBody(struct kevent* curr_event)
{
    int read_content_body_res = IDLE;
    size_t total_content_body_length = fd_manager_[curr_event->ident].getRequest().getHeaders().getContentLength();

    if (total_content_body_length == 0)
        read_content_body_res = READ_FINISH_REQUEST;
    else
    {
        if (fd_content_[curr_event->ident].size() >= total_content_body_length)
        {
            std::vector<char>::iterator it = fd_content_[curr_event->ident].begin();
            std::vector<char>           content_split(it, it + total_content_body_length);
            std::string                 body = charVectorToString(content_split);

            fd_manager_[curr_event->ident].getRequest().setBody(body);

            fd_content_[curr_event->ident].erase(it, it + total_content_body_length);
            fd_manager_[curr_event->ident].setFdContentIndex(0);

            read_content_body_res = READ_FINISH_REQUEST;
        }
        else if (fd_content_[curr_event->ident].size() < total_content_body_length)
        {
            read_content_body_res = IDLE;
        }
    }
    return (read_content_body_res);
}

// Chunked Length를 읽는 함수
void    KeventHandler::readChunkedLength(struct kevent* curr_event, std::string chunk_split)
{
    int chunked_length_decimal = hexToDecimal(chunk_split.c_str(), chunk_split.size());

    fd_manager_[curr_event->ident].setChunkedTotalReadLength(chunked_length_decimal);
    fd_manager_[curr_event->ident].setChunkedDataType(CHUNKED_DATA);
    fd_manager_[curr_event->ident].setChunkedCrLf(0);

    std::cout << "token: " << chunked_length_decimal << "\n";
}

// Chunked Data를 읽는 함수
void    KeventHandler::readChunkedData(struct kevent* curr_event, std::string chunk_split)
{

    // if (fd_manager_[curr_event->ident].getChunkedTotalReadLength() == 0 && fd_manager_[curr_event->ident].getChunkedCrLf() == 2)
    //     read_chunked_data_res = READ_FINISH_REQUEST;
    // else
    // {
    fd_manager_[curr_event->ident].getRequest().addBody(chunk_split);
    fd_manager_[curr_event->ident].setChunkedDataType(CHUNKED_LENGTH);
    fd_manager_[curr_event->ident].setChunkedCrLf(0);
    // }
    

    // if (fd_manager_[curr_event->ident].getContentCurrentReadLength() < fd_manager_[curr_event->ident].getChunkedTotalReadLength())
    // {
    //     // std::cout << buf[i];
    //     // fd_content_[curr_event->ident].push_back(buf[i]);
    //     fd_manager_[curr_event->ident].setContentCurrentReadLength();

    //     if (fd_manager_[curr_event->ident].getContentCurrentReadLength() == fd_manager_[curr_event->ident].getChunkedTotalReadLength())
    //     {
    //         // sooha += fd_manager_[curr_event->ident].getContentCurrentReadLength();
    //         // std::cout << "read chuncked: " << j++ << " size: " << sooha << "\n";
    //         fd_manager_[curr_event->ident].setContentCurrentReadLength(0);
    //         // fd_manager_[curr_event->ident].setChunkedDataType(CHUNKED_LENGTH);
    //         fd_manager_[curr_event->ident].setChunkedDataType(DONE_DATA);
    //     }
    // }
}

// Chunked 타입의 Body를 읽는 함수
int KeventHandler::readChunkedBody(struct kevent* curr_event)
{
    int read_chunked_body_res = IDLE;
    int crlf_size = 2;

    // printCharVectorCRLF(fd_content_[curr_event->ident]);
    while (1)
    {
        size_t  i = fd_manager_[curr_event->ident].getFdContentIndex();
        for (; i < fd_content_[curr_event->ident].size(); i++)
        {
            // std::cout << fd_content_[curr_event->ident][i] << "\n";
            // if (fd_content_[curr_event->ident][i] == '\r')
            //     std::cout << "@@\n";
            if (fd_content_[curr_event->ident].at(i) == '\r' && fd_manager_[curr_event->ident].getChunkedCrLf() == 0)
                fd_manager_[curr_event->ident].setChunkedCrLf(1);
            else if (fd_content_[curr_event->ident].at(i) == '\n' && fd_manager_[curr_event->ident].getChunkedCrLf() == 1)
            {
                fd_manager_[curr_event->ident].setChunkedCrLf(2);
                //
                if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_DATA && fd_manager_[curr_event->ident].getChunkedCurrentReadLength() < fd_manager_[curr_event->ident].getChunkedTotalReadLength())
                {
                    fd_manager_[curr_event->ident].setChunkedCrLf(0);
                    fd_manager_[curr_event->ident].incChunkedCurrentReadLength();
                    continue;
                }
                //
                i++;
                break ;
            }
            else
                fd_manager_[curr_event->ident].setChunkedCrLf(0);
            
            if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_DATA)
                fd_manager_[curr_event->ident].incChunkedCurrentReadLength();
            else
                fd_manager_[curr_event->ident].setChunkedCurrentReadLength(0);
        }
        if (fd_manager_[curr_event->ident].getChunkedCrLf() != 2)
        {
            fd_manager_[curr_event->ident].setFdContentIndex(i);
            break ;
        }
        if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_DATA
            && fd_manager_[curr_event->ident].getChunkedTotalReadLength() == 0 && fd_manager_[curr_event->ident].getChunkedCrLf() == 2)
        {
            std::cout << "chunked body end\n";
            return (READ_FINISH_REQUEST);
        }    

        std::vector<char>::iterator   it = fd_content_[curr_event->ident].begin();
        std::vector<char>   content_split(it, it + (i - crlf_size));
        std::string chunk_split = charVectorToString(content_split);
        fd_content_[curr_event->ident].erase(it, it + i);
        fd_manager_[curr_event->ident].setFdContentIndex(0);

        // std::cout << "idx: " << i << "\n";
        // std::cout << "chunked_split_size: " << chunk_split.size() << "\n";
        // std::cout << "chunked_split: " << chunk_split << "\n";


        if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_LENGTH)
            readChunkedLength(curr_event, chunk_split);
        else if (fd_manager_[curr_event->ident].getChunkedDataType() == CHUNKED_DATA)
            readChunkedData(curr_event, chunk_split);
    }
    return (read_chunked_body_res);
}

// 나누어져서 들어오는 Request 메시지를 처리하고 Body 형식이 Chunked, Content-length에 따라서 그에 적합한 처리를 수행하는 함수
int KeventHandler::addSegmentReqAndReadMode(struct kevent* curr_event)
{
    int res = IDLE;

    if (fd_manager_[curr_event->ident].getReadType() == READ_HEADER)
    {
        // std::cout << "READ_HEADER\n";
        res = readReqHeader(curr_event);
        if (res == IDLE)
            return (res);
    }

    if (fd_manager_[curr_event->ident].getReadType() == NO_EXIST_BODY)
    {
        // std::cout << "READ_FINISH_REQUEST\n";
        res = READ_FINISH_REQUEST;
    }
    else if (fd_manager_[curr_event->ident].getReadType() == READ_CONTENT_LENGTH_BODY)
    {
        // std::cout << "READ_CONTENT_LENGTH_BODY\n";
        res = readContentBody(curr_event);
    }
    else if (fd_manager_[curr_event->ident].getReadType() == READ_CHUNKED_BODY)
    {
        // std::cout << "READ_CHUNKED_BODY\n";
        res = readChunkedBody(curr_event);
    }
    
    return (res);
}

std::string parsingCgiBody(std::string str)
{
    std::string buf;
    std::istringstream streamLine(str);

    std::getline(streamLine, buf);
    std::getline(streamLine, buf);
    std::getline(streamLine, buf);
    std::getline(streamLine, buf);
    return (buf);
}

bool KeventHandler::isPipeFile(unsigned int file_fd)
{
    std::string str;

    if (fd_manager_[file_fd].getCgiStatus() != READ_CGI)
        return (false);
    str = parsingCgiBody(charVectorToString(fd_content_[file_fd]));
    fd_manager_[fd_manager_[file_fd].getParentClientFd()].getRequest().setBody(str);
    fd_manager_[file_fd].setCgiStatus(DONE_CGI);
    close(file_fd);
    return (true);
}

int KeventHandler::readFdFlag(struct kevent* curr_event)
{
    char    buf[BUFFER_SIZE];
    int     n = 0;

    std::cout << "pipe read!\n";
    if (fd_content_.find(curr_event->ident) != fd_content_.end())
    {
        memset(buf, 0, BUFFER_SIZE);
        n = read(curr_event->ident, buf, BUFFER_SIZE - 1);
        if (n < 0 && (curr_event->flags & EV_EOF))
            return (CLOSE_CONNECTION);
        if (n < 0)
            return (READ_ERROR);
        // buf[*n] = '\0';
        addContent(curr_event, buf, n);
        if (fd_manager_[curr_event->ident].getEventReadFile() == -1)
            return (addSegmentReqAndReadMode(curr_event));
        else
        {
            if (isPipeFile(curr_event->ident))
                return (READ_FINISH_REQUEST);
            return (READ_FINISH_FILE);
        }
        // fd_content에서 crlf 확인하기
        
        // if (curr_event->data <= *n)
    //     if (//crlf가 들어왔다면)
    //     {
    //         if (fd_manager_[curr_event->ident].getEventReadFile() == -1)
    //             return (addSegmentReqAndReadMode(curr_event, buf, *n));
    //         else
    //         {
    //             addContent(curr_event, buf, *n);
    //             if (isPipeFile(curr_event->ident))
    //                 return (READ_FINISH_REQUEST);
    //             return (READ_FINISH_FILE);
    //         }
    //     }
    //     else
    //         return (READ_CONTINUE);
    }
    if (curr_event->flags & EV_EOF)
        return (CLOSE_CONNECTION);
    return (IDLE);
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
            return (EDIT_FILE);
        else
            return (IDLE);
    }
    return (-1);
}

int  KeventHandler::getEventFlag(struct kevent* curr_event)
{
    // std::cout << "cur_fd: " << curr_event->ident << "\n";
    // std::cout << "flag: " << fd_manager_[curr_event->ident].getCgiStatus() << "\n";

    std::cout << "occur event! 1\n";
    if (fd_manager_.find(curr_event->ident) == fd_manager_.end())
        return (IDLE);
    std::cout << "occur event! 2\n";

    // cgi
    if (fd_manager_[curr_event->ident].getCgiStatus() == WRITE_CGI)
        return (WRITE_CGI);
    std::cout << "occur event! 3\n";

    if (curr_event->flags & EV_ERROR)
    {
        std::cout << "occur event! ev_error\n";
        return (ERROR);
    }
    else if (curr_event->filter == EVFILT_READ || fd_manager_[curr_event->ident].getCgiStatus() == READ_CGI)
    {
        std::cout << "read fd flag\n";
        if (isSocket(curr_event))
            return (IS_SERVER_SOCKET);
        return (readFdFlag(curr_event));
    }
    else if (curr_event->filter == EVFILT_WRITE)
    {
        std::cout << "write fd flag\n";
        return (writeFdFlag(curr_event));
    }
    
    std::cout << "occur event! 4\n";
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
            // std::cout << "curr_event->ident : " << curr_event->ident << std::endl;
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
    // std::cout << "add content\n";
    fd_content_[curr_event->ident].insert(fd_content_[curr_event->ident].end(), buf, buf + n);
}

void KeventHandler::closePipes(int parent_fd)
{
    close(fd_manager_[parent_fd].getSendPipe(1));
    fd_manager_.erase(fd_manager_[parent_fd].getSendPipe(1));

    close(fd_manager_[parent_fd].getSendPipe(0));
    fd_manager_.erase(fd_manager_[parent_fd].getSendPipe(0));

    close(fd_manager_[parent_fd].getReceivePipe(1));
    fd_manager_.erase(fd_manager_[parent_fd].getReceivePipe(1));
}

void KeventHandler::connectPipe(int parent_fd)
{
    dup2(fd_manager_[parent_fd].getSendPipe(0), 0);
    dup2(fd_manager_[parent_fd].getReceivePipe(1), 1);
    close(fd_manager_[parent_fd].getSendPipe(0));
    close(fd_manager_[parent_fd].getSendPipe(1));
    close(fd_manager_[parent_fd].getReceivePipe(0));
    close(fd_manager_[parent_fd].getReceivePipe(1));
}

char** KeventHandler::createEnv(int parent_fd)
{
    char **env;

    env = new char *[5];
    std::string str0 = "REQUEST_METHOD=" + fd_manager_[parent_fd].getRequest().getRequestLine().getMethod();
    std::string str1 = "SERVER_PROTOCOL=" + fd_manager_[parent_fd].getRequest().getRequestLine().getVersion();
    std::string str2 = "PATH_INFO=/";
    std::string str3 = "CONTENT_LENGTH=" + std::to_string(fd_manager_[parent_fd].getRequest().getBody().size());
    env[0] = new char[str0.size() + 1];
    strcpy(env[0], str0.c_str());
    env[1] = new char[str1.size() + 1];
    strcpy(env[1], str1.c_str());
    env[2] = new char[str2.size() + 1];
    strcpy(env[2], str2.c_str());
    env[3] = new char[str3.size() + 1];
    strcpy(env[3], str3.c_str());
    env[4] = NULL;

    return (env);
}

void KeventHandler::createPipe(int parent_fd)
{
    int fd[2];

    pipe(fd);
    EventRecorder pipe_event_recorder(parent_fd);
    fd_manager_[fd[1]] = pipe_event_recorder;
    fd_manager_[fd[0]] = pipe_event_recorder;

    fd_manager_[parent_fd].setReceivePipe(0, fd[0]);
    fd_manager_[parent_fd].setReceivePipe(1, fd[1]);
    fd_manager_[fd[0]].setCgiStatus(READ_CGI);
}

// void writeData(int fd, const std::string& data) {
//     const char* buffer = data.c_str(); // 문자열 데이터를 버퍼로 변환
//     size_t totalBytesWritten = 0; // 쓰여진 총 바이트 수

//     while (totalBytesWritten < data.size()) {
//         // 남은 데이터의 일부분을 쓰기
//         int bytesToWrite = data.size() - totalBytesWritten;
//         if (bytesToWrite > 1024) { // 예를 들어 1024 바이트씩 쓰기
//             bytesToWrite = 1024;
//         }

//         int bytesWritten = write(fd, buffer + totalBytesWritten, bytesToWrite);
//         if (bytesWritten < 0) {
//             // write 호출 실패
//             // 에러 처리 코드 추가
//             break;
//         }

//         totalBytesWritten += bytesWritten; // 쓰여진 바이트 수 업데이트
//     }
// }

//cgi pipe 쓰기 fd
void KeventHandler::executeCgi(struct kevent* curr_event)
{
    pid_t pid;
    int parent_fd = fd_manager_[curr_event->ident].getParentClientFd();

    std::cout << "occur event! 2.1\n";
    fd_manager_[curr_event->ident].setCgiStatus(-1);
    std::cout << "body size : " << fd_manager_[parent_fd].getRequest().getBody().size() << std::endl;
    // std::cout << "body : " << "[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[" << fd_manager_[parent_fd].getRequest().getBody();
    // std::cout << "]]]]]]]]]]]]]]]]]]]]]]]]]]]\n";
    // writeData(curr_event->ident, fd_manager_[parent_fd].getRequest().getBody());
    int n = write(curr_event->ident, fd_manager_[parent_fd].getRequest().getBody().c_str(), fd_manager_[parent_fd].getRequest().getBody().size());
    // int n = write(curr_event->ident, fd_manager_[parent_fd].getRequest().getBody().c_str(), 100);
    if (n < 0)
    {
        write(2, "occur event! 2.1 erorr\n", 24);
        std::runtime_error("cgi write error\n");
    }
    std::cout << "occur event! 2.2\n";
    createPipe(parent_fd);
    std::cout << "occur event! 2.3\n";
    pid = fork();
    if (pid == 0)
    {
        std::cout << "occur event! 2.3.1 child\n";
        char **env;
        char *path[2];

        env = createEnv(parent_fd);
        path[0] = (char*)fd_manager_[parent_fd].getCgiPath().c_str();
        path[1] = 0;
        connectPipe(parent_fd);
        if (execve((char*)fd_manager_[parent_fd].getCgiPath().c_str(), path, env) < 0)
            std::runtime_error("error\n");
    }
    else
    {
        std::cout << "occur event! 2.3.1 parents\n";
        closePipes(parent_fd);
        waitpid(-1, 0, 0);
        fd_manager_[fd_manager_[parent_fd].getReceivePipe(0)].setCgiStatus(READ_CGI);
        fd_manager_[fd_manager_[parent_fd].getReceivePipe(0)].setEventReadFile(1);   // 이건 파일이다.
        fd_content_[fd_manager_[parent_fd].getReceivePipe(0)];
        changeEvents(change_list_, fd_manager_[parent_fd].getReceivePipe(0), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
        std::cout << "run!\n";
    }
    std::cout << "occur event! 3\n";
}

int KeventHandler::transferFd(uintptr_t fd)
{
    // 파이프 파일이면 부모 fd 리턴
    if (fd_manager_[fd].getCgiStatus() == DONE_CGI)
    {
        int parent_fd;

        parent_fd = fd_manager_[fd].getParentClientFd();
        fd_manager_[parent_fd].setCgiStatus(DONE_CGI);
        fd_manager_.erase(fd);
        fd_content_.erase(fd);
        return (parent_fd);
    }
    // 아니면 자기 자신 리턴
    return (fd);
}

void KeventHandler::runServer(void)
{
    int             new_events;
    int             event_type;
    struct kevent*  curr_event;
    // char            buf[BUFFER_SIZE];
    // int             n;

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
            // event_type = getEventFlag(curr_event, buf, &n);
            event_type = getEventFlag(curr_event);

            switch(event_type)
            {
                case WRITE_CGI:
                    executeCgi(curr_event);
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

                // case READ_CONTINUE :
                //     addContent(curr_event, buf, n);
                //     break ;

                case SEND_RESPONSE :
                    sendResponse(curr_event->ident);
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
