#ifndef EVENTRECORDER_HPP
# define EVENTRECORDER_HPP

# include "Request.hpp"
# include "Response.hpp"

class EventRecorder
{
private:
    int event_read_file_;
    // int event_read_req_;
    int event_write_res_;
    int event_write_pipe_;
    int event_write_file_;
    int parent_client_fd_;
    int fd_error_;
    int file_offset_;
    Request req_;
    Response res_;

public:
    EventRecorder();
    EventRecorder(int parent_client_fd);
    ~EventRecorder();

    void setFileOffset(int n);
    void setEventReadFile(int flag);
    // void setEventReadReq(int flag);
    void setEventWriteRes(int flag);
    void setEventWritePipe(int flag);
    void setEventWriteFile(int flag);
    void setParentClientFd(int fd);
    void setFdError(int flag);
    void setRequest(Request& req);
    void setResponse(Response& res);

    int getEventReadFile(void);
    // int getEventReadReq(void);
    int getEventWriteRes(void);
    int getEventWritePipe(void);
    int getEventWriteFile(void);
    int getParentClientFd(void);
    int getFdError(void);
    Request& getRequest();
    Response& getResponse();
};

#endif