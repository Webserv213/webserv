#include "KeventHandler.hpp"

KeventHandler::KeventHandler(Http &http): http_(http)
{

}

KeventHandler::~KeventHandler()
{

}

void    KeventHandler::disconnectClient(int client_fd, std::map< int, std::vector<char> >& data)
{
    std::cout << "client disconnected: " << client_fd << std::endl;
    close(client_fd);
    data.erase(client_fd);
    fd_manager_.erase(client_fd);
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
    (*server_addr).sin_port = htons(server.getListen("127.0.0.1"));
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

        if (listen(listen_socket_fd, 5) == -1)
            throw(std::runtime_error("listen() error\n"));
        fcntl(listen_socket_fd, F_SETFL, O_NONBLOCK);

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
            fcntl(client_socket, F_SETFL, O_NONBLOCK);

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

void    KeventHandler::createRequest(struct kevent* curr_event)
{
    Request req;

    std::string temp;
    std::istringstream streamLine(charVectorToString(fd_content_[curr_event->ident]));

    std::getline(streamLine, temp, ' ');
    req.getRequestLine().setMethod(temp);
    std::getline(streamLine, temp, ' ');
    req.getRequestLine().setRequestTarget(temp);
    std::getline(streamLine, temp);
    req.getRequestLine().setVersion(temp);
    while (1)
    {
        std::getline(streamLine, temp, ' ');
        if (temp.find("Host:") != std::string::npos) {
            req.getHeaders().setHost(temp);
            break ;
        }
    }
    fd_manager_[curr_event->ident].setRequest(req);

    std::string url;
    if (req.getRequestLine().getMethod() == "GET")
    {
        url = http_.getServer()[0].getRoot() + req.getRequestLine().getRequestTarget();
    }
    if (url != "./var/www/favicon.ico")
    {
        url += "/index.html";
    }

    int fd;
    fd = open (url.c_str(), O_RDONLY);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    if (fd < 0)
    {
        std::cout << "fd_error : " << fd << std::endl;
        exit(0);
    }
    fd_content_[fd];
    EventRecorder st(curr_event->ident);
    st.setEventReadFile(1);
    fd_manager_[fd] = st;
    changeEvents(change_list_, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[fd]);
    changeEvents(change_list_, curr_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    fd_manager_[curr_event->ident].setEventWriteRes(-1);
}

void KeventHandler::createResponse(struct kevent* curr_event)
{
    int parent_fd = fd_manager_[curr_event->ident].getParentClientFd();
    std::string version = fd_manager_[parent_fd].getRequest().getRequestLine().getVersion();

    fd_manager_[parent_fd].getResponse().getStatusLine().setVersion(version);
    fd_manager_[parent_fd].getResponse().getStatusLine().setStatusCode("200");
    fd_manager_[parent_fd].getResponse().getStatusLine().setStatusText("OK");
    fd_manager_[parent_fd].getResponse().getHeaders().setServer("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setKeepAlive("timeout=100");

    time_t rawTime;
    time(&rawTime);

    struct tm *timeInfo;
    timeInfo = gmtime(&rawTime);

    char buffer[80];
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);

    fd_manager_[parent_fd].getResponse().getHeaders().setDate(buffer);

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget() != "/favicon.ico")
        fd_manager_[parent_fd].getResponse().getHeaders().setContentType("text/html");
    else
        fd_manager_[parent_fd].getResponse().getHeaders().setContentType("image/x-icon");

    fd_manager_[parent_fd].getResponse().getHeaders().setLastModified("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setTransferEncoding("default");
    fd_manager_[parent_fd].getResponse().getHeaders().setConnection("keep-alive");
    fd_manager_[parent_fd].getResponse().getHeaders().setContentEncoding("default");

    if (fd_manager_[parent_fd].getRequest().getRequestLine().getRequestTarget() != "/favicon.ico")
        fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(fd_content_[curr_event->ident].size()));
    else
        fd_manager_[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(1150));

    std::string file_data(charVectorToString(fd_content_[curr_event->ident]));
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

void    KeventHandler::sendResponse(struct kevent* curr_event)
{
    int n = write(curr_event->ident, &(*fd_content_[curr_event->ident].begin()), fd_content_[curr_event->ident].size());
    if (n == -1)
    {
        std::cerr << "client write error!" << std::endl;
        disconnectClient(curr_event->ident, fd_content_);
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

bool KeventHandler::isSocket(struct kevent* curr_event)
{
     for(size_t i = 0; i < server_sockets_.size(); i++)
    {
        if (curr_event->ident == server_sockets_[i])
            return (true);
    }
    return (false);
}

int KeventHandler::readFdFlag(struct kevent* curr_event, char *buf, int *n)
{
    if (fd_content_.find(curr_event->ident) != fd_content_.end())
    {
        memset(buf, 0, BUFFER_SIZE);
        *n = read(curr_event->ident, buf, BUFFER_SIZE);
        buf[*n] = '\0';

        if (*n < 0)
            return (READ_ERROR);
        else if (curr_event->data == *n)
        {
            addContent(curr_event, buf, *n);
            if (fd_manager_[curr_event->ident].getEventReadFile() == -1)
            {
                if (fd_content_[curr_event->ident].size() != 0)
                    return (READ_FINISH_REQUEST);
            }
            else
                return (READ_FINISH_FILE);
        }
        else
            return (READ_CONTINUE);
    }
    return (-1);
}

int  KeventHandler::writeFdFlag(struct kevent* curr_event)
{
    std::map<int, std::vector<char> >::iterator it = fd_content_.find(curr_event->ident);
    if (it != fd_content_.end())
    {
        if (fd_content_[curr_event->ident].size() != 0 && fd_manager_[curr_event->ident].getEventWriteRes() == 1)
            return (SEND_RESPONSE);
        else
            return (EDIT_FILE);
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
            disconnectClient(curr_event->ident, fd_content_);
        }
    }
}

void KeventHandler::clientReadError(struct kevent* curr_event)
{
    std::cerr << "client read error!" <<std::endl;
    disconnectClient(curr_event->ident, fd_content_);
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
                    break ;

                default :
                    throw(std::runtime_error("event exception error\n"));
            }
        }
    }
}