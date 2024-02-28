#include "Status.hpp"

Status::Status()
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

Status::Status(int parent_client_fd)
{
    event_read_file_ = -1;
    // event_read_req_ = -1;
    event_write_res_ = -1;
    event_write_pipe_ = -1;
    event_write_file_ = -1;
    parent_client_fd_ = parent_client_fd;
    fd_error_ = -1;
}

Status::~Status()
{

}

void Status::setFileOffset(int n)
{
    file_offset_ += n;
}

void Status::setEventReadFile(int flag)
{
    event_read_file_ = flag;
}

// void Status::setEventReadReq(int flag)
// {
//     event_read_req_ = flag;
// }

void Status::setEventWriteRes(int flag)
{
    event_write_res_ = flag;
}

void Status::setEventWritePipe(int flag)
{
    event_write_pipe_ = flag;
}

void Status::setEventWriteFile(int flag)
{
    event_write_file_ = flag;
}

void Status::setParentClientFd(int fd)
{
    parent_client_fd_ = fd;
}

void Status::setFdError(int flag)
{
    fd_error_ = flag;
}

void Status::setRequest(Request& req)
{
    req_ = req;
}

void Status::setResponse(Response& res)
{
    res_ = res;
}

int Status::getEventReadFile(void)
{
    return (event_read_file_);
}

// int Status::getEventReadReq(void)
// {
//     return (event_read_req_);
// }

int Status::getEventWriteRes(void)
{
    return (event_write_res_);
}

int Status::getEventWritePipe(void)
{
    return (event_write_pipe_);
}

int Status::getEventWriteFile(void)
{
    return (event_write_file_);
}

int Status::getParentClientFd(void)
{
    return (parent_client_fd_);
}

int Status::getFdError(void)
{
    return (fd_error_);
}

Request& Status::getRequest()
{
    return (req_);
}

Response& Status::getResponse()
{
    return (res_);
}
