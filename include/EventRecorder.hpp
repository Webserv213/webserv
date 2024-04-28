#ifndef EVENTRECORDER_HPP
# define EVENTRECORDER_HPP

# include "Request.hpp"
# include "Response.hpp"

enum    ReadType
{
    READ_HEADER,
    READ_CONTENT_LENGTH_BODY,
    READ_CHUNKED_BODY,
    NO_EXIST_BODY,
};

enum    ChunkedFlag
{
    CHUNKED_LENGTH,
    CHUNKED_DATA,
};

class EventRecorder
{
private:
    Request req_;
    Response res_;

    bool event_read_file_;
    bool event_write_res_;

    bool header_crlf_; 
    bool chunked_cr_lf_;
    bool chunked_data_type_;

    int parent_fd_;
    int read_type_;

    int send_pipe_[2];
    int receive_pipe_[2];
    int cgi_status_;

    std::string cgi_path_;
    std::string send_cgi_body_;
    
    size_t fd_content_index_;

    size_t content_current_read_length_;

    size_t chunked_begin_idx_;
    size_t chunked_current_read_length_;
    size_t chunked_total_read_length_;

    size_t write_body_idx_;

public:
    EventRecorder();
    EventRecorder(int parent_fd);
    ~EventRecorder();

    //setter
    void setRequest(Request& req);
    void setResponse(Response& res);

    void setEventReadFile(bool flag);
    void setEventWriteRes(bool flag);

    void setHeaderCrLf(bool flag);
    void setChunkedCrLf(bool flag);
    void setChunkedDataType(bool type);

    void setParentFd(int fd);
    void setReadType(int type);

    void setSendPipe(int idx, int fd);
    void setReceivePipe(int idx, int fd);
    void setCgiStatus(int status);

    void setCgiPath(std::string path);
    void setSendCgiBody(const std::string& body);

    void setFdContentIndex(size_t idx);

    void setContentCurrentReadLength(size_t length);
    
    void setChunkedBeginIndex(size_t idx);
    void setChunkedCurrentReadLength(size_t length);
    void setChunkedTotalReadLength(size_t length);

    void setWriteBodyIndex(size_t idx);

    //getter
    Request& getRequest();
    Response& getResponse();

    bool getEventReadFile(void);
    bool getEventWriteRes(void);

    bool getHeaderCrLf(void);
    bool getChunkedCrLf(void);
    bool getChunkedDataType(void);

    int getParentFd(void);
    int getReadType(void);

    int getSendPipe(int idx);
    int getReceivePipe(int idx);
    int getCgiStatus(void);

    std::string getCgiPath(void);
    std::string& getSendCgiBody(void);

    size_t getFdContentIndex(void);

    size_t getContentCurrentReadLength(void);

    size_t getChunkedBeginIndex();
    size_t getChunkedCurrentReadLength(void);
    size_t getChunkedTotalReadLength(void);

    size_t getWriteBodyIndex(void);

    //utils
    void sumWriteBodyIndex(size_t idx);
};

#endif
