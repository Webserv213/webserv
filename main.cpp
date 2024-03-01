#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "Http.hpp"

#include <iostream>
#include <map>
#include <vector>
#include "Request.hpp"
#include "Response.hpp"
#include <sstream>
#include <ctime>
#include <cstdio>

#include "Server.hpp"

#include "KeventHandler.hpp"

#define PORT1 8082

// std::map<int, bool> m;

// void    print_vectorChar(std::vector<char>& vector_char)
// {
//     for (size_t i = 0; i < vector_char.size(); i++)
//         std::cout << vector_char[i];
// }

// std::vector<char> string_to_vectorChar(std::string& str)
// {
//     std::vector<char> vector_char;
//     for (size_t i = 0; i < str.size(); i++)
//         vector_char.push_back(str[i]);
//     return (vector_char);
// }

// std::string vectorChar_to_string(std::vector<char>& vector_char)
// {
//     std::string str;
//     for (size_t i = 0; i < vector_char.size(); i++)
//         str += vector_char[i];
//     return (str);
// }

// void exit_with_perror(const std::string& msg)
// {
//     std::cerr << msg << std::endl;
//     exit(EXIT_FAILURE);
// }

// void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
//         uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
// {
//     struct kevent temp_event;

//     EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
//     change_list.push_back(temp_event);
// }

// void disconnect_client(int client_fd, std::map<int, std::vector<char> >& clients)
// {
//     std::cout << "client disconnected: " << client_fd << std::endl;
//     close(client_fd);
//     clients.erase(client_fd);
// }

int main()
{
    Http http;
    // parsing config file(&http);
    Server server;
    http.setServer(server);
    // http.setServer(server);
    KeventHandler keventhandler(http);
    

    // openListenSocket()
    try
    {
        keventhandler.openListenSocket();
        keventhandler.tmp();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }
    
    /* init kqueue */
    // int kq;
    // if ((kq = kqueue()) == -1)
    //     exit_with_perror("kqueue() error\n" + std::string(strerror(errno)));


    // /* main loop */
    // int new_events;
    // struct kevent* curr_event;
    // while (1)
    // {
    //     new_events = kevent(kq, &change_list[0], change_list.size(), event_list, 100, NULL);
    //     if (new_events == -1)
    //         exit_with_perror("kevent() error\n" + std::string(strerror(errno)));

    //     change_list.clear();

    //     for (int i = 0; i < new_events; ++i)
    //     {
    //         curr_event = &event_list[i];

    //         /* check error event return */
    //         if (curr_event->flags & EV_ERROR)
    //         {
    //             std::cout << "error_\n" << std::endl;
    //             exit(0);
    //             // if (curr_event->ident == server_socket)
    //             //     exit_with_perror("server socket error");
    //             // else
    //             // {
    //             //     cerr << "client socket error" << endl;
    //             //     disconnect_client(curr_event->ident, clients);
    //             // }
    //         }
    //         else if (curr_event->filter == EVFILT_READ)
    //         {
    //             int flag = false;
    //             for(size_t i = 0; i < server_sockets.size(); i++)
    //             {
    //                 if (curr_event->ident == server_sockets[i])
    //                 {
    //                     /* accept new client */
    //                     int client_socket;
    //                     if ((client_socket = accept(server_sockets[i], NULL, NULL)) == -1)
    //                         exit_with_perror("accept() error\n" + std::string(strerror(errno)));
    //                     std::cout << "accept new client: " << client_socket << std::endl;
    //                     fcntl(client_socket, F_SETFL, O_NONBLOCK);

    //                     /* add event for client socket - add read && write event */
    //                     Status st;
    //                     fd_manager[client_socket] = st;
    //                     change_events(change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager[client_socket]);
    //                     change_events(change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager[client_socket]);
    //                     clients[client_socket];
    //                     flag = true;
    //                     // std::cout << "serversocket : " << server_sockets[i] << ", client_socket: " << client_socket << std::endl;
    //                 }
    //             }
    //             if (flag == true)
    //                 continue;
    //             if (clients.find(curr_event->ident)!= clients.end())
    //             {
    //                 /* read data from client */
    //                 char buf[100000];
    //                 memset(buf, 0, sizeof(buf));
    //                 int n = read(curr_event->ident, buf, sizeof(buf));
    //                 buf[n] = '\0';
    //                 // std::cout << "--" << curr_event->ident << "----" << n << "----------\n" << std::endl;
    //                 // write(1, buf, n);
    //                 // std::cout << "bufff: " << buf << " , n : "<< n << "filefd: " << curr_event->ident << "--\n";
    //                 if (n < 0)
    //                 {
    //                     if (n < 0)
    //                     {
    //                         std::cerr << "client read error!" <<std::endl;
    //                     }
    //                     disconnect_client(curr_event->ident, clients);
    //                 }
    //                 // else if (n == 0)
    //                 else if (curr_event->data == n)
    //                 {
    //                     // vector<char>.insert()
    //                     clients[curr_event->ident].insert(clients[curr_event->ident].end(), buf, buf + n);
    //                     if (fd_manager[curr_event->ident].getEventReadFile() == -1)
    //                     {
    //                         // std::cout << "0--------requset---------" << "client_socket: " << curr_event->ident << "--\n";
    //                         // std::cout << clients[curr_event->ident] << std::endl;
    //                         // std::cout << "1--------requset-----------\n";


    //                         if (clients[curr_event->ident].size() != 0)
    //                         {   
    //                             //----------parsing----------
    //                             Request req;

    //                             std::string temp;
    //                             std::istringstream streamLine(vectorChar_to_string(clients[curr_event->ident]));

    //                             std::getline(streamLine, temp, ' ');
    //                             req.getRequestLine().setMethod(temp);
    //                             std::getline(streamLine, temp, ' ');
    //                             req.getRequestLine().setRequestTarget(temp);
    //                             std::getline(streamLine, temp);
    //                             req.getRequestLine().setVersion(temp);
    //                             while (1)
    //                             {
    //                                 std::getline(streamLine, temp, ' ');
    //                                 if (temp.find("Host:") != std::string::npos) {
    //                                     req.getHeaders().setHost(temp);
    //                                     break ;
    //                                 }
    //                             }
    //                             fd_manager[curr_event->ident].setRequest(req);
    //                             //----------parsing----------

    //                             //----------GET 수행----------
    //                             std::string url;
    //                             if (req.getRequestLine().getMethod() == "GET")
    //                             {
    //                                 url = server.getRoot() + req.getRequestLine().getRequestTarget();
    //                                 // std::cout << "-----------------GET-------------------\n";
    //                                 // std::cout << url;
    //                             }
    //                             if (url != "./var/www/favicon.ico") 
    //                             {
    //                                 url += "/index.html";
    //                                 // std::cout << "sooha.html client_sockt : " << curr_event->ident << std::endl;
    //                             }
    //                             else
    //                             {
    //                                 // std::cout << "favicon client_sockt : " << curr_event->ident << std::endl;
    //                             }
    //                             // std::cout << "-----------------GET-------------------\n";
    //                             // std::cout << url;
    //                             int fd;
    //                             fd = open (url.c_str(), O_RDONLY);
    //                             fcntl(fd, F_SETFL, O_NONBLOCK);
    //                             // std::cout << "url: " << url << ", fd : " << fd << std::endl;

    //                             // if (url == "./var/www/favicon.ico") 
    //                             // {
    //                             //     char buf[100000];
    //                             //     int n = read(fd, buf, sizeof(buf));
    //                             //     buf[n] = '\0';
    //                             //     std::cout << "read: " << buf << "\n";
    //                             // }
    //                             // std::cout << "file_fd : " << fd << std::endl;
    //                             if (fd < 0)
    //                             {
    //                                 std::cout << "fd_error : " << fd << std::endl;
    //                                 exit(0);
    //                             }
    //                             clients[fd];
    //                             Status st(curr_event->ident);
    //                             st.setEventReadFile(1);
    //                             fd_manager[fd] = st;
    //                             change_events(change_list, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager[fd]);
    //                             //----------GET 수행----------
    //                             change_events(change_list, curr_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    //                             fd_manager[curr_event->ident].setEventWriteRes(-1);
    //                         }
    //                     }
    //                     else   // response 만들기
    //                     {
    //                         // print_vectorChar(clients[curr_event->ident]);
    //                         // std::cout << std::endl;
    //                         // *이 블록의 curr_event는 file의 fd이다.
    //                         // curr_event의 부모인 fd의 res구조체에 response할 데이터 세팅하는 코드

    //                         // std::cout << "index.html: " << clients[curr_event->ident] << "\n";

    //                         // status_line
    //                         int parent_fd = fd_manager[curr_event->ident].getParentClientFd();
    //                         std::string version = fd_manager[parent_fd].getRequest().getRequestLine().getVersion();

    //                         fd_manager[parent_fd].getResponse().getStatusLine().setVersion(version); // -> ㅈ식의 리스펀스
    //                         // 버버전은 d_manager[curr_event->ident].getParentCLientFd

    //                         fd_manager[parent_fd].getResponse().getStatusLine().setStatusCode("200");
    //                         fd_manager[parent_fd].getResponse().getStatusLine().setStatusText("OK");
                
    //                         // header
    //                         fd_manager[parent_fd].getResponse().getHeaders().setServer("default");
    //                         fd_manager[parent_fd].getResponse().getHeaders().setKeepAlive("timeout=100");

    //                         //get time
    //                         time_t rawTime;
    //                         time(&rawTime);
    //                         // time_t 값을 GMT 기준으로 tm 구조체로 변환합니다.
    //                         struct tm *timeInfo;
    //                         timeInfo = gmtime(&rawTime);
    //                         // 날짜와 시간을 지정된 형식으로 변환합니다.
    //                         char buffer[80];
    //                         strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeInfo);

    //                         fd_manager[parent_fd].getResponse().getHeaders().setDate(buffer);
    //                         // std::cout << "target : " << fd_manager[parent_fd].getRequest().getRequestLine().getRequestTarget() << std::endl;
    //                         if (fd_manager[parent_fd].getRequest().getRequestLine().getRequestTarget() != "/favicon.ico")
    //                             fd_manager[parent_fd].getResponse().getHeaders().setContentType("text/html");
    //                         else
    //                             fd_manager[parent_fd].getResponse().getHeaders().setContentType("image/x-icon");
    //                         fd_manager[parent_fd].getResponse().getHeaders().setLastModified("default");
    //                         fd_manager[parent_fd].getResponse().getHeaders().setTransferEncoding("default");
    //                         fd_manager[parent_fd].getResponse().getHeaders().setConnection("keep-alive");
    //                         fd_manager[parent_fd].getResponse().getHeaders().setContentEncoding("default");
    //                         if (fd_manager[parent_fd].getRequest().getRequestLine().getRequestTarget() != "/favicon.ico")
    //                             fd_manager[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(clients[curr_event->ident].size()));
    //                         else
    //                             fd_manager[parent_fd].getResponse().getHeaders().setContentLength(std::to_string(1150));

    //                         // body
    //                         std::string file_data(vectorChar_to_string(clients[curr_event->ident]));
    //                         fd_manager[parent_fd].getResponse().setBody(file_data);
    //                         // response ready

    //                         std::string res_tmp;
    //                         res_tmp = "";
    //                         // std::cout << "------------------------------------" << fd_manager[parent_fd].getResponse().getHeaders().getContentLength() << "----------------------------" << std::endl;
    //                         fd_manager[parent_fd].getResponse().getStatusLine().setVersion("HTTP/1.1");
    //                         res_tmp += (fd_manager[parent_fd].getResponse().getStatusLine().getVersion() + " ");
    //                         res_tmp += (fd_manager[parent_fd].getResponse().getStatusLine().getStatusCode() + " ");
    //                         res_tmp += (fd_manager[parent_fd].getResponse().getStatusLine().getStatusText() + "\r\n");
    //                         res_tmp += ("Date: " + fd_manager[parent_fd].getResponse().getHeaders().getDate() + "\n");
    //                         // res_tmp += ("Sever: " + fd_manager[parent_fd].getResponse().getHeaders().getServer() + "\n");
    //                         res_tmp += ("Content-Type: " + fd_manager[parent_fd].getResponse().getHeaders().getContentType() + "\n");
    //                         // res_tmp += ("Last-Modified: " + fd_manager[parent_fd].getResponse().getHeaders().getLastModified() + "\n");
    //                         res_tmp += ("Content-Length: " + fd_manager[parent_fd].getResponse().getHeaders().getContentLength() + "\n");
    //                         res_tmp += ("Connection: " + fd_manager[parent_fd].getResponse().getHeaders().getConnection() + "\n");
    //                         res_tmp += ("Keep-Alive: " + fd_manager[parent_fd].getResponse().getHeaders().getKeepAlive() + "\r\n");
    //                         // res_tmp += ("\r\n" + fd_manager[parent_fd].getResponse().getBody());
    //                         res_tmp += "\r\n" + vectorChar_to_string(fd_manager[parent_fd].getResponse().getBody());
    //                         clients[parent_fd] = string_to_vectorChar(res_tmp);

    //                         fd_manager[parent_fd].setEventWriteRes(1);
    //                         close(curr_event->ident);
    //                         clients.erase(curr_event->ident);
    //                         fd_manager.erase(curr_event->ident);
    //                         change_events(change_list, parent_fd, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, &fd_manager[parent_fd]);
    //                         // std::cout << "우리가 만든 response: \n" << clients[parent_fd] << std::endl;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     clients[curr_event->ident].insert(clients[curr_event->ident].end(), buf, buf + n);
    //                     // std::cout << "kevent->data: " << curr_event->data << ", bufsize : " << n << std::endl;
    //                     // std::cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident] << std::endl;
    //                 }
    //             }
    //         }
    //         else if (curr_event->filter == EVFILT_WRITE)
    //         {
    //             /* send data to client */
    //             std::map<int, std::vector<char> >::iterator it = clients.find(curr_event->ident);
    //             if (it != clients.end())
    //             {
    //                 if (clients[curr_event->ident].size() != 0 && fd_manager[curr_event->ident].getEventWriteRes() == 1)
    //                 {
    //                     int n = write(curr_event->ident, &(*clients[curr_event->ident].begin()), clients[curr_event->ident].size());
    //                     if (n == -1)
    //                     {
    //                         std::cerr << "client write error!" << std::endl;
    //                         disconnect_client(curr_event->ident, clients);
    //                         fd_manager[curr_event->ident].setFdError(1);
    //                     }
    //                     else {
    //                         // clients.erase(curr_event->ident);
    //                         clients[curr_event->ident].clear();
    //                         // clients[curr_event->ident].erase();
    //                         fd_manager[curr_event->ident].setEventWriteRes(-1);
    //                         change_events(change_list, curr_event->ident, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    //                         change_events(change_list, curr_event->ident, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, &fd_manager[curr_event->ident]);
    //                         // 엄마 Init
    //                         Status n;
    //                         fd_manager[curr_event->ident] = n;
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    return (0);
}
