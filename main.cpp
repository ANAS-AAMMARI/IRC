#include "server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (!isNumber(argv[1]) || strlen(argv[2]) == 0)
    {
        if (!isNumber(argv[1]))
            std::cerr << "Invalid port number" << std::endl;
        else
            std::cerr << "password cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    Server server(strtoll(argv[1], NULL, 10), argv[2]);
    server.run();

    return 0;
}