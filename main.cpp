#include "Http.hpp"
#include "Server.hpp"
#include "KeventHandler.hpp"

// void initDummyServer(Http &http)
// {
//     std::string server_name;
//     std::string ip;

//     Server server;
//     server_name = "wooseong.webserv.com";
//     ip = "127.0.0.1";
//     server.setServerName(server_name);
//     server.setListen(ip, 8081);
//     http.setServer(server);

//     Server server2;
//     server_name = "seoji.webserv.com";
//     ip =
//     server2.setServerName("seoji.webserv.com");
//     server2.setListen("127.0.0.1", 8082);
//     http.setServer(server2);

//     Server server3;
//     server3.setServerName("sooha.webserv.com");
//     server3.setListen("127.0.0.1", 8083);
//     http.setServer(server3);
// }

int main(int argc, char **argv)
{
    Http http;
    // Server server;
    // http.setServer(server);
    // initDummyServer(http);

    try
    {
        http.setWebserv(argc, argv);
        KeventHandler keventhandler(http);
        keventhandler.openListenSocket();
        keventhandler.runServer();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        exit(10);
    }
    return (0);
}
