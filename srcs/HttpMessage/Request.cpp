#include "Request.hpp"

Request::Request(void)
{
    body_ = "";
}

Request::~Request(void)
{
    
}

RequestLine& Request::getRequestLine()
{
    return (start_line_);
}

ReqHeaders& Request::getHeaders()
{
    return (headers_);
}

std::string& Request::getBody(void)
{
    return (body_);
}

void Request::addBody(std::string& body)
{
    body_ += body;
}

void Request::setBody(std::string& body)
{
    body_ = body;
}

// void Request::methodGetHandler(Server &server, )
// {
//     std::string file_path;
//     if (getRequestLine().getRequestTarget() == "/favicon.ico")
//     {
//         file_path = "./var/www/favicon.ico";
//     }
//     else
//     {
//         file_path = server.getRoot() + getRequestLine().getRequestTarget() + "/index.html";
//     }

//     int fd;
//     fd = open (file_path.c_str(), O_RDONLY);
//     fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
//     if (fd < 0)
//     {
//         std::cout << file_path + " fd_error : " << fd << std::endl;
//         exit(0);
//     }
//     fd_content_[fd];
//     EventRecorder event_recorder(curr_event->ident);
//     event_recorder.setEventReadFile(1);
//     fd_manager_[fd] = event_recorder;
//     changeEvents(change_list_, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager_[fd]);
//     changeEvents(change_list_, curr_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
//     fd_manager_[curr_event->ident].setEventWriteRes(-1);
// }