#include "EventRecorder.hpp"

EventRecorder::EventRecorder()
{
    event_read_file_ = -1;
    // event_read_req_ = -1;
    event_write_res_ = -1;
    event_write_pipe_ = -1;
    event_write_file_ = -1;
    parent_client_fd_ = -1;
    fd_error_ = -1;
    file_offset_ = 0;
}

EventRecorder::EventRecorder(int parent_client_fd)
{
    event_read_file_ = -1;
    // event_read_req_ = -1;
    event_write_res_ = -1;
    event_write_pipe_ = -1;
    event_write_file_ = -1;
    file_offset_ = 0;
    parent_client_fd_ = parent_client_fd;
    fd_error_ = -1;
}

EventRecorder::~EventRecorder()
{

}

void EventRecorder::setFileOffset(int n)
{
    file_offset_ += n;
}

void EventRecorder::setEventReadFile(int flag)
{
    event_read_file_ = flag;
}

// void EventRecorder::setEventReadReq(int flag)
// {
//     event_read_req_ = flag;
// }

void EventRecorder::setEventWriteRes(int flag)
{
    event_write_res_ = flag;
}

void EventRecorder::setEventWritePipe(int flag)
{
    event_write_pipe_ = flag;
}

void EventRecorder::setEventWriteFile(int flag)
{
    event_write_file_ = flag;
}

void EventRecorder::setParentClientFd(int fd)
{
    parent_client_fd_ = fd;
}

void EventRecorder::setFdError(int flag)
{
    fd_error_ = flag;
}

void EventRecorder::setRequest(Request& req)
{
    req_ = req;
}

void EventRecorder::setResponse(Response& res)
{
    res_ = res;
}

int EventRecorder::getEventReadFile(void)
{
    return (event_read_file_);
}

// int EventRecorder::getEventReadReq(void)
// {
//     return (event_read_req_);
// }

int EventRecorder::getEventWriteRes(void)
{
    return (event_write_res_);
}

int EventRecorder::getEventWritePipe(void)
{
    return (event_write_pipe_);
}

int EventRecorder::getEventWriteFile(void)
{
    return (event_write_file_);
}

int EventRecorder::getParentClientFd(void)
{
    return (parent_client_fd_);
}

int EventRecorder::getFdError(void)
{
    return (fd_error_);
}

Request& EventRecorder::getRequest()
{
    return (req_);
}

Response& EventRecorder::getResponse()
{
    return (res_);
}
