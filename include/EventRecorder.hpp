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

enum PipeMode
{
    READ_PIPE,
    WRITE_PIPE,
};

class EventRecorder
{
private:
    int event_read_file_;
    int event_write_res_;
    int event_write_pipe_;
    int event_write_file_;
    int parent_client_fd_;
    int fd_error_;
    int file_offset_;
    int autoindex_;
    int read_type_; // reading header or content-longth body or chunked body
    int header_eof_; // cr - lf - cr -lf 가 연속으로 들어오는지 기록
    int content_current_read_length_; //content lentth 길이
    int chunked_eof_;   //chunked data의 끝 확인 플래그
    int chunked_cr_lf_; // chunked data의 cr lf 확인 플래그
    int chunked_data_type_;    // chunked data에서 길이인지 데이터인지 확인하는 플래그
    int chunked_current_read_length_;
    int chunekd_total_read_length_;
    std::string cgi_path_;
    int cgi_status_;
    int send_pipe_[2];
    int Receive_pipe_[2];
    int pipe_mode_;

    std::vector<char>   chunked_length_temp_;
    Request req_;
    Response res_;

public:
    EventRecorder();
    EventRecorder(int parent_client_fd);
    ~EventRecorder();

    void setFileOffset(int n);
    void setEventReadFile(int flag);
    void setEventWriteRes(int flag);
    void setEventWritePipe(int flag);
    void setEventWriteFile(int flag);
    void setParentClientFd(int fd);
    void setFdError(int flag);
    void setAutoindexFlag(int flag);
    void setReadType(int read_type);
    void setHeaderEof(int header_eof);
    void setContentCurrentReadLength(int content_length_cnt);
    void incContentCurrentReadLength(void);
    void setChunkedEof(int chunked_eof);
    void setChunkedCrLf(int chunked_cr_lf);
    void setChunkedDataType(int chunked_data_type);
    void setChunkedCurrentReadLength(int chunked_current_read_length);
    void setChunkedTotalReadLength(int chunked_total_read_length);
    void setRequest(Request req);
    void setResponse(Response& res);
    void setChunkedLengthTemp(std::vector<char> chunked_length_temp);
    void setSendPipe(int idx, int fd);
    void setReceivePipe(int idx, int fd);
    void setCgiStatus(int status);
    void setCgiPath(std::string path);
    void setPipeMode(int pipe_mode);

    int getEventReadFile(void);
    int getEventWriteRes(void);
    int getEventWritePipe(void);
    int getEventWriteFile(void);
    int getParentClientFd(void);
    int getFdError(void);
    int getAutoindexFlag(void);
    int getReadType(void);
    int getHeaderEof(void);
    int getContentCurrentReadLength(void);
    int getChunkedEof(void);
    int getChunkedCrLf(void);
    int getChunkedDataType(void);
    int getChunkedCurrentReadLength(void);
    int getChunkedTotalReadLength(void);
    int getSendPipe(int idx);
    int getReceivePipe(int idx);
    int getCgiStatus(void);
    std::string getCgiPath(void);
    int getPipeMode(void);

    // utils
    void pushbackChunkedLengthTemp(char c);
    void clearChunkedLengthTemp(void);
    void incChunkedCurrentReadLength(void);
    std::vector<char>& getChunkedLengthTemp();
    Request& getRequest();
    Response& getResponse();
};

#endif