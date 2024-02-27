#include "Status.hpp"

Status::Status()
{
    root_fd_ = -1;
    open_file_ = -1;
    read_req_ = -1;
    write_res_ = -1;
    req_ = NULL;
    res_ = NULL;
}

Status::Status(int root_fd)
{
    root_fd_ = root_fd;
    open_file_ = -1;
    read_req_ = -1;
    write_res_ = -1;
    req_ = NULL;
    res_ = NULL;
}

Status::~Status()
{


}

void Status::setOpenFile(int fd)
{
    open_file_ = fd;
}

void Status::setReadReq(int flag)
{
    read_req_ = flag;
}

void Status::setWriteRes(int flag)
{
    write_res_ = flag;
}

void Status::setRequest(Request* req)
{
    req_ = req;
}

void Status::setResponse(Response* res)
{
    res_ = res;
}

int Status::getRootFd()
{
    return (root_fd_);
}

int Status::getOpenFile()
{
    return (open_file_);
}

int Status::getReadReq()
{
    return (read_req_);
}

int Status::getWriteRes()
{
    return (write_res_);
}

Request* Status::getRequest()
{
    return (req_);
}

Response* Status::getResponse()
{
    return (res_);
}
