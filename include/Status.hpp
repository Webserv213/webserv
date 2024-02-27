#ifndef STATUS_HPP
# define STATUS_HPP

# include "Request.hpp"
# include "Response.hpp"

class Status
{
private:
    int open_file_;
    int read_req_;
    int write_res_;
    Request *req_;
    Response *res_;
    int root_fd_;

public:
    Status();
    Status(int root_fd);
    ~Status();
    void setOpenFile(int fd);
    void setReadReq(int flag);
    void setWriteRes(int flag);
    void setRequest(Request* req);
    void setResponse(Response* res);
    int getRootFd();
    int getOpenFile();
    int getReadReq();
    int getWriteRes();
    Request* getRequest();
    Response* getResponse();
};

#endif