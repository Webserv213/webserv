#include "EventRecorder.hpp"

EventRecorder::EventRecorder()
{
    event_read_file_ = -1;
    event_write_res_ = -1;
    event_write_pipe_ = -1;
    event_write_file_ = -1;
    parent_client_fd_ = -1;
    fd_error_ = -1;
    file_offset_ = 0;
    autoindex_ = -1;
    read_type_ = READ_HEADER;
    header_eof_ = 0;
    content_current_read_length_ = 0;
    chunked_eof_ = 0;
    chunked_cr_lf_ = 0;
    chunked_data_type_ = CHUNKED_LENGTH;
    chunked_current_read_length_ = 0;
    chunekd_total_read_length_ = 0;
}

EventRecorder::EventRecorder(int parent_client_fd)
{
    event_read_file_ = -1;
    event_write_res_ = -1;
    event_write_pipe_ = -1;
    event_write_file_ = -1;
    file_offset_ = 0;
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

void EventRecorder::setChunkedLengthTemp(std::vector<char> chunked_length_temp)
{
    chunked_length_temp_ = chunked_length_temp;
}

void EventRecorder::pushbackChunkedLengthTemp(char c)
{
    chunked_length_temp_.push_back(c);
}

void EventRecorder::clearChunkedLengthTemp(void)
{
    chunked_length_temp_.clear();
}


void EventRecorder::setRequest(Request req)
{
    req_ = req;
}

void EventRecorder::setResponse(Response& res)
{
    res_ = res;
}

// getter

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

std::vector<char>& EventRecorder::getChunkedLengthTemp()
{
    return (chunked_length_temp_);
}


Request& EventRecorder::getRequest()
{
    return (req_);
}

Response& EventRecorder::getResponse()
{
    return (res_);
}