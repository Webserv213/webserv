#include "Http.hpp"
#include "Server.hpp"
#include "KeventHandler.hpp"

int main(int argc, char **argv)
{
    Http http;

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
