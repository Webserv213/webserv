#include "EventRecorder.hpp"

EventRecorder::EventRecorder()
{
    event_read_file_ = -1;
    event_write_res_ = -1;
    event_write_pipe_ = -1;
    event_write_file_ = -1;
    parent_client_fd_ = -1;
    fd_error_ = -1;
    fd_content_index_ = 0;
    autoindex_ = -1;
    read_type_ = READ_HEADER;
    header_eof_ = 0;
    content_current_read_length_ = 0;
    chunked_eof_ = 0;
    chunked_cr_lf_ = 0;
    chunked_data_type_ = CHUNKED_LENGTH;
    chunked_current_read_length_ = 0;
    chunekd_total_read_length_ = 0;
    pipe_mode_ = -1;
    write_body_idx_ = 0;
    remain_write_cgi_data_ = 0;
}

EventRecorder::EventRecorder(int parent_client_fd)
{
    event_read_file_ = -1;
    event_write_res_ = -1;
    event_write_pipe_ = -1;
    event_write_file_ = -1;
    fd_content_index_ = 0;
    parent_client_fd_ = parent_client_fd;
    fd_error_ = -1;
    autoindex_ = -1;
    read_type_ = READ_HEADER;
    header_eof_ = 0;
    content_current_read_length_ = 0;
    chunked_eof_ = 0;
    chunked_cr_lf_ = 0;
    chunked_data_type_ = CHUNKED_LENGTH;
    chunked_current_read_length_ = 0;
    chunekd_total_read_length_ = 0;
    pipe_mode_ = -1;
    write_body_idx_ = 0;
    remain_write_cgi_data_ = 0;
}

EventRecorder::~EventRecorder()
{

}

void EventRecorder::setFdContentIndex(int n)
{
    fd_content_index_ = n;
}

void EventRecorder::setEventReadFile(int flag)
{
    event_read_file_ = flag;
}

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

void EventRecorder::setAutoindexFlag(int flag)
{
    autoindex_ = flag;
}


void EventRecorder::setReadType(int read_type)
{
    read_type_ = read_type;
}

void EventRecorder::setHeaderEof(int header_eof)
{
    header_eof_ = header_eof;
}

void EventRecorder::setContentCurrentReadLength(int content_length_cnt)
{
    content_current_read_length_ = content_length_cnt;
}

void EventRecorder::incContentCurrentReadLength(void)
{
    content_current_read_length_++;
}

void EventRecorder::incChunkedCurrentReadLength(void)
{
    chunked_current_read_length_++;
}

void    EventRecorder::setChunkedEof(int chunked_eof)
{
    chunked_eof_ = chunked_eof;
}

void    EventRecorder::setChunkedCrLf(int chunked_cr_lf)
{
    chunked_cr_lf_ = chunked_cr_lf;
}

void    EventRecorder::setChunkedDataType(int chunked_data_type)
{
    chunked_data_type_ = chunked_data_type;
}


void    EventRecorder::setChunkedCurrentReadLength(int chunked_current_read_length)
{
    chunked_current_read_length_ = chunked_current_read_length;
}

void    EventRecorder::setChunkedTotalReadLength(int chunked_total_read_length)
{
    chunekd_total_read_length_ = chunked_total_read_length;
}

void EventRecorder::setRequest(Request req)
{
    req_.bodyClear();
    req_ = req;
}

void EventRecorder::setResponse(Response& res)
{
    res_ = res;
}

void EventRecorder::setSendPipe(int idx, int fd)
{
    send_pipe_[idx] = fd;
}

void EventRecorder::setReceivePipe(int idx, int fd)
{
    Receive_pipe_[idx] = fd;
}

void EventRecorder::setCgiStatus(int status)
{
    cgi_status_ = status;
}

void EventRecorder::setCgiPath(std::string path)
{
    cgi_path_ = path;
}

void EventRecorder::setPipeMode(int pipe_mode)
{
    pipe_mode_ = pipe_mode;
}

void EventRecorder::setSendCgiBody(std::string send_cgi_body)
{
    send_cgi_body_.clear();
    send_cgi_body_ = send_cgi_body;
}

void EventRecorder::setWriteBodyIndex(int idx)
{
    write_body_idx_ = idx;
}

void EventRecorder::sumWriteBodyIndex(int idx)
{
    write_body_idx_ += idx;
}

void EventRecorder::setRemainWriteCgiData(int n)
{
    remain_write_cgi_data_ = n;
}


// getter

size_t EventRecorder::getFdContentIndex(void)
{
    return (fd_content_index_);
}

int EventRecorder::getEventReadFile(void)
{
    return (event_read_file_);
}

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

int EventRecorder::getAutoindexFlag(void)
{
    return (autoindex_);
}

int EventRecorder::getReadType(void)
{
    return (read_type_);
}

int EventRecorder::getHeaderEof(void)
{
    return (header_eof_);
}

int EventRecorder::getContentCurrentReadLength(void)
{
    return (content_current_read_length_);
}

int EventRecorder::getChunkedEof(void)
{
    return (chunked_eof_);
}

int EventRecorder::getChunkedCrLf(void)
{
    return (chunked_cr_lf_);
}

int EventRecorder::getChunkedDataType(void)
{
    return (chunked_data_type_);
}

int EventRecorder::getChunkedCurrentReadLength(void)
{
    return (chunked_current_read_length_);
}

int EventRecorder::getChunkedTotalReadLength(void)
{
    return (chunekd_total_read_length_);
}

int EventRecorder::getSendPipe(int idx)
{
    return (send_pipe_[idx]);
}

int EventRecorder::getReceivePipe(int idx)
{
    return (Receive_pipe_[idx]);
}

int EventRecorder::getCgiStatus(void)
{
    return (cgi_status_);
}

std::string EventRecorder::getCgiPath(void)
{
    return (cgi_path_);
}

int EventRecorder::getPipeMode(void)
{
    return (pipe_mode_);
}

std::string EventRecorder::getSendCgiBody(void)
{
    return (send_cgi_body_);
}

Request& EventRecorder::getRequest()
{
    return (req_);
}

Response& EventRecorder::getResponse()
{
    return (res_);
}

int EventRecorder::getWriteBodyIndex(void)
{
    return (write_body_idx_);
}

int EventRecorder::getRemainWriteCgiData(void)
{
    return (remain_write_cgi_data_);
}