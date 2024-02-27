#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <map>
#include <vector>
#include "Request.hpp"
#include "Response.hpp"
#include <sstream>

#include "Server.hpp"

#include "Status.hpp"

std::map<int, bool> m;

void exit_with_perror(const std::string& msg)
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

void change_events(std::vector<struct kevent>& change_list, uintptr_t ident, int16_t filter,
        uint16_t flags, uint32_t fflags, intptr_t data, void *udata)
{
    struct kevent temp_event;

    EV_SET(&temp_event, ident, filter, flags, fflags, data, udata);
    change_list.push_back(temp_event);
}

void disconnect_client(int client_fd, std::map<int, std::string>& clients)
{
    std::cout << "client disconnected: " << client_fd << std::endl;
    close(client_fd);
    clients.erase(client_fd);
}

int main()
{
    // conf file 파싱
    // Http http;
    Server server;  // test
    std::vector<Status> a;

    std::vector<Request> requests;

    std::vector<uintptr_t> server_sockets;
    std::map<int, std::string> clients;
    std::vector<struct kevent> change_list;
    struct kevent event_list[8];

    for(int i = 0; i < 2; i++)
    {
        int server_socket;
        struct sockaddr_in server_addr;
        if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
            exit_with_perror("socket() error\n" + std::string(strerror(errno)));

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (i == 0)
            server_addr.sin_port = htons(server.getListen("127.0.0.1"));   
        else
            server_addr.sin_port = htons(85);   
        if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
            exit_with_perror("bind() error\n" + std::string(strerror(errno)));

        if (listen(server_socket, 5) == -1)
            exit_with_perror("listen() error\n" + std::string(strerror(errno)));
        fcntl(server_socket, F_SETFL, O_NONBLOCK);
        server_sockets.push_back(server_socket);
        Status *status = new Status();  //delete not yet
        change_events(change_list, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, status);
        std::cout << "echo server started" << std::endl;
    }
    
    /* init kqueue */
    int kq;
    if ((kq = kqueue()) == -1)
        exit_with_perror("kqueue() error\n" + std::string(strerror(errno)));


    /* main loop */
    int new_events;
    struct kevent* curr_event;
    while (1)
    {
        new_events = kevent(kq, &change_list[0], change_list.size(), event_list, 8, NULL);
        if (new_events == -1)
            exit_with_perror("kevent() error\n" + std::string(strerror(errno)));

        change_list.clear();

        for (int i = 0; i < new_events; ++i)
        {
            curr_event = &event_list[i];

            /* check error event return */
            if (curr_event->flags & EV_ERROR)
            {
                // if (curr_event->ident == server_socket)
                //     exit_with_perror("server socket error");
                // else
                // {
                //     cerr << "client socket error" << endl;
                //     disconnect_client(curr_event->ident, clients);
                // }
            }
            else if (curr_event->filter == EVFILT_READ)
            {
                int flag = false;
                for(size_t i = 0; i < server_sockets.size(); i++)
                {
                    if (curr_event->ident == server_sockets[i])
                    {
                        /* accept new client */
                        int client_socket;
                        if ((client_socket = accept(server_sockets[i], NULL, NULL)) == -1)
                            exit_with_perror("accept() error\n" + std::string(strerror(errno)));
                        std::cout << "accept new client: " << client_socket << std::endl;
                        fcntl(client_socket, F_SETFL, O_NONBLOCK);

                        /* add event for client socket - add read && write event */
                        Status *status = new Status();  // delete not yet
                        change_events(change_list, client_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, status);
                        Status *status2 = new Status();  //delete not yet
                        change_events(change_list, client_socket, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, status2);
                        clients[client_socket] = "";
                        flag = true;
                    }
                }
                if (flag == true)
                    continue;
                if (clients.find(curr_event->ident)!= clients.end())
                {
                    /* read data from client */
                    char buf[1024];
                    int n = read(curr_event->ident, buf, sizeof(buf));

                    if (n < 0)
                    {
                        if (n < 0)
                        {
                            std::cerr << "client read error!" <<std::endl;
                        }
                        disconnect_client(curr_event->ident, clients);
                    }
                    else if (n == 0)
                    {
                        if ((static_cast<Status*>(curr_event->udata))->getOpenFile() == -1)
                        {
                            (static_cast<Status*>(curr_event->udata))->setWriteRes(1);
                            if (clients[curr_event->ident] != "")
                            {
                                // request 요청 파싱
                                Request* req = new Request();
                                
                                //----------parsing----------

                                std::string temp;
                                std::istringstream streamLine(clients[curr_event->ident]);

                                std::getline(streamLine, temp, ' ');
                                req->getRequestLine().setMethod(temp);
                                std::getline(streamLine, temp, ' ');
                                req->getRequestLine().setRequestTarget(temp);
                                std::getline(streamLine, temp);
                                req->getRequestLine().setVersion(temp);
                                while (1)
                                {
                                    std::getline(streamLine, temp, ' ');
                                    if (temp.find("Host:") != std::string::npos) {
                                        req->getHeaders().setHost(temp);
                                        break ;
                                    }
                                }
                                //----------parsing----------

                                //----------GET 수행----------
                                std::string url;
                                if (req->getRequestLine().getMethod() == "GET")
                                {
                                    url = server.getRoot() + req->getRequestLine().getRequestTarget();
                                    std::cout << "-----------------GET-------------------\n";
                                    std::cout << url;
                                }
                                url += "/index.html";
                                int fd;
                                fd = open (url.c_str(), O_RDONLY);
                                clients[fd] = "";
                                Status *status = new Status(curr_event->ident);  //delete not yet
                                status->setOpenFile(fd);
                                change_events(change_list, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, status);
                                //----------GET 수행----------
                                delete static_cast<Status*>(curr_event->udata);
                                change_events(change_list, curr_event->ident, EVFILT_READ, EV_DELETE, 0, 0, NULL);

                                (static_cast<Status*>(curr_event->udata))->setWriteRes(0);
                            }
                        }
                        else   // response 만들기
                        {
                            char buf2[100000];
                            int n = read (curr_event->ident, buf2, sizeof(buf2));
                            buf2[n] = '\0';

                            std::cout << "buf2: " << buf2 << "\n";

                            Response *res = new Response();
                            // status_line
                            res->getStatusLine().setVersion(static_cast<Status*>(curr_event->udata)->getRequest()->getRequestLine().getVersion());
                            res->getStatusLine().setStatusCode("200");
                            res->getStatusLine().setStatusText("OK");
                
                            // header
                            res->getHeaders().setServer("default");
                            res->getHeaders().setDate("2024-02-27");
                            res->getHeaders().setContentType("default");
                            res->getHeaders().setLastModified("default");
                            res->getHeaders().setTransferEncoding("default");
                            res->getHeaders().setConnection("default");
                            res->getHeaders().setContentEncoding("default");

                            // body
                            std::string a(buf2);
                            res->setBody(a);

                            int fd = static_cast<Status*>(curr_event->udata)->getRootFd();

                            static_cast<Status*>(curr_event->udata)->setResponse(res);
                            static_cast<Status*>(fd)->setWriteRes(1);    // client 의 fd
                            static_cast<Status*>(curr_event->udata)->setOpenFile(-1);
                        }
                    }
                    else
                    {
                        buf[n] = '\0';
                        clients[curr_event->ident] += buf;
                        // std::cout << "received data from " << curr_event->ident << ": " << clients[curr_event->ident] << std::endl;
                    }
                }
            }
            else if (curr_event->filter == EVFILT_WRITE)
            {
                /* send data to client */
                std::map<int, std::string>::iterator it = clients.find(curr_event->ident);
                if (it != clients.end())
                {
                    if (clients[curr_event->ident] != "" && (static_cast<Status*>(curr_event->udata))->getWriteRes() == 1)
                    {
                        std::cout << static_cast<Status*>(curr_event->udata)->getResponse()->getBody() << "\n";
                        int n;
                        if ((n = write(curr_event->ident, clients[curr_event->ident].c_str(),
                                        clients[curr_event->ident].size()) == -1))
                        {
                            std::cerr << "client write error!" << std::endl;
                            disconnect_client(curr_event->ident, clients);

                            (static_cast<Status*>(curr_event->udata))->setWriteRes(0);
                        }
                        else
                            clients[curr_event->ident].clear();
                    }
                }
            }
        }
    }
    return (0);
}
