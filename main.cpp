#include "Http.hpp"
#include "Server.hpp"
#include "KeventHandler.hpp"

int main()
{
    Http http;
    // parsing config file(&http);
    Server server;
    http.setServer(server);
    KeventHandler keventhandler(http);

    try
    {
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
