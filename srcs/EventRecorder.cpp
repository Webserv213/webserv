#include "EventRecorder.hpp"

EventRecorder::EventRecorder()
{
    event_read_file_ = 0;
    event_write_res_ = 0;

    header_crlf_ = 0;
    chunked_cr_lf_ = 0;
    chunked_data_type_ = CHUNKED_LENGTH;

    parent_fd_ = -1;
    read_type_ = READ_HEADER;

    fd_content_index_ = 0;

    content_current_read_length_ = 0;

    chunked_begin_idx_ = 0;
    chunked_current_read_length_ = 0;
    chunked_total_read_length_ = 0;

    write_body_idx_ = 0;
}

EventRecorder::EventRecorder(int parent_fd)
{
    event_read_file_ = 0;
    event_write_res_ = 0;

    header_crlf_ = 0;
    chunked_cr_lf_ = 0;
    chunked_data_type_ = CHUNKED_LENGTH;

    parent_fd_ = parent_fd;
    read_type_ = READ_HEADER;

    fd_content_index_ = 0;

    content_current_read_length_ = 0;

    chunked_begin_idx_ = 0;
    chunked_current_read_length_ = 0;
    chunked_total_read_length_ = 0;

    write_body_idx_ = 0;
}

EventRecorder::~EventRecorder()
{

}

//setter 
void EventRecorder::setRequest(Request& req)
{
    req_ = req;
}

void EventRecorder::setResponse(Response& res)
{
    res_ = res;
}

void EventRecorder::setEventReadFile(bool flag)
{
    event_read_file_ = flag;
}

void EventRecorder::setEventWriteRes(bool flag)
{
    event_write_res_ = flag;
}

void EventRecorder::setHeaderCrLf(bool flag)
{
    header_crlf_ = flag;
}

void    EventRecorder::setChunkedCrLf(bool flag)
{
    chunked_cr_lf_ = flag;
}

void    EventRecorder::setChunkedDataType(bool type)
{
    chunked_data_type_ = type;
}

void EventRecorder::setParentFd(int fd)
{
    parent_fd_ = fd;
}

void EventRecorder::setReadType(int type)
{
    read_type_ = type;
}

void EventRecorder::setSendPipe(int idx, int fd)
{
    send_pipe_[idx] = fd;
}

void EventRecorder::setReceivePipe(int idx, int fd)
{
    receive_pipe_[idx] = fd;
}

void EventRecorder::setCgiStatus(int status)
{
    cgi_status_ = status;
}

void EventRecorder::setCgiPath(std::string path)
{
    cgi_path_ = path;
}

void EventRecorder::setSendCgiBody(const std::string& body)
{
    send_cgi_body_ = body;
}

void EventRecorder::setFdContentIndex(size_t idx)
{
    fd_content_index_ = idx;
}

void EventRecorder::setContentCurrentReadLength(size_t cnt)
{
    content_current_read_length_ = cnt;
}

void EventRecorder::setChunkedBeginIndex(size_t idx)
{
    chunked_begin_idx_ = idx;
}

void    EventRecorder::setChunkedCurrentReadLength(size_t chunked_current_read_length)
{
    chunked_current_read_length_ = chunked_current_read_length;
}

void    EventRecorder::setChunkedTotalReadLength(size_t chunked_total_read_length)
{
    chunked_total_read_length_ = chunked_total_read_length;
}

void EventRecorder::setWriteBodyIndex(size_t idx)
{
    write_body_idx_ = idx;
}

// getter
Request& EventRecorder::getRequest()
{
    return (req_);
}

Response& EventRecorder::getResponse()
{
    return (res_);
}

bool EventRecorder::getEventReadFile(void)
{
    return (event_read_file_);
}

bool EventRecorder::getEventWriteRes(void)
{
    return (event_write_res_);
}

bool EventRecorder::getHeaderCrLf(void)
{
    return (header_crlf_);
}

bool EventRecorder::getChunkedCrLf(void)
{
    return (chunked_cr_lf_);
}

bool EventRecorder::getChunkedDataType(void)
{
    return (chunked_data_type_);
}

int EventRecorder::getParentFd(void)
{
    return (parent_fd_);
}

int EventRecorder::getReadType(void)
{
    return (read_type_);
}

int EventRecorder::getSendPipe(int idx)
{
    return (send_pipe_[idx]);
}

int EventRecorder::getReceivePipe(int idx)
{
    return (receive_pipe_[idx]);
}

int EventRecorder::getCgiStatus(void)
{
    return (cgi_status_);
}

std::string EventRecorder::getCgiPath(void)
{
    return (cgi_path_);
}

std::string& EventRecorder::getSendCgiBody(void)
{
    return (send_cgi_body_);
}

size_t EventRecorder::getFdContentIndex(void)
{
    return (fd_content_index_);
}

size_t EventRecorder::getContentCurrentReadLength(void)
{
    return (content_current_read_length_);
}

size_t  EventRecorder::getChunkedBeginIndex(void)
{
    return (chunked_begin_idx_);
}

size_t EventRecorder::getChunkedCurrentReadLength(void)
{
    return (chunked_current_read_length_);
}

size_t EventRecorder::getChunkedTotalReadLength(void)
{
    return (chunked_total_read_length_);
}

size_t EventRecorder::getWriteBodyIndex(void)
{
    return (write_body_idx_);
}

//utils
void EventRecorder::sumWriteBodyIndex(size_t idx)
{
    write_body_idx_ += idx;
}