#include "server.hpp"

const int Server::BUFFER_SIZE = 1024;

Server::Server(long long port, std::string password)
{
    this->setPort(port);
    this->password = password;
}

Server &Server::operator=(const Server &other)
{
    if (this != &other)
    {
        this->port = other.port;
        this->password = other.password;
        this->pollfds = other.pollfds;
        this->clients = other.clients;
    }
    return *this;
}

Server::Server(const Server &other)
{
    *this = other;
}

Server::~Server() {}

void Server::setPort(long long port)
{
    if (port > 65535 || port < 1024)
    {
        std::cerr << "Invalid port number" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->port = port;
}

void Server::run()
{
    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // allow socket to be reuseable
    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Error setting socket options" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking
    if (fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "Error setting socket to non-blocking" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    if (inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr) < 0)
    {
        std::cerr << "Error converting IP address" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error binding socket to port or address." << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSocket, SOMAXCONN) < 0)
    {
        std::cerr << "Error listening for connections" << std::endl;
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Add server socket to pollfds
    struct pollfd serverPollfd = {serverSocket, POLLIN, 0};
    pollfds.push_back(serverPollfd);

    // Poll for events
    while (true)
    {
        // wait for events
        int pollStatus = poll(&pollfds[0], pollfds.size(), -1);
        if (pollStatus < 0)
        {
            std::cerr << "Error polling for events" << std::endl;
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        // Check for new connections
        if (pollfds[0].revents & POLLIN)
            handleNewConnection();

        // Check for client events
        for (size_t i = 1; i < pollfds.size(); i++)
        {

            if (pollfds[i].revents & POLLHUP)
                handleClientDisconnection(pollfds[i].fd);
            else if (pollfds[i].revents & POLLIN)
            {
                std::cout << "Client event " << pollfds[i].revents << std::endl;
                handelClientMessage(pollfds[i].fd);
            }
        }
    }

    // Close server socket
    close(serverSocket);
}

void Server::handleNewConnection()
{
    // Accept new connection
    struct sockaddr_in clientAddress;
    socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocket = accept(pollfds[0].fd, (struct sockaddr *)&clientAddress, &clientAddressSize);
    if (clientSocket < 0)
    {
        std::cerr << "Error accepting new connection" << std::endl;
        close(pollfds[0].fd);
        exit(EXIT_FAILURE);
    }

    // Set socket to non-blocking
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "Error setting socket to non-blocking" << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // Add client to pollfds
    struct pollfd clientPollfd = {clientSocket, POLLIN, 0};
    pollfds.push_back(clientPollfd);

    // Add client to clients
    struct client newClient = {clientSocket, ""};
    clients[clientSocket] = newClient;

    // Send welcome message
    std::string welcomeMessage = "Welcome to the chat server!\n";
    if (send(clientSocket, welcomeMessage.c_str(), welcomeMessage.size(), 0) < 0)
    {
        std::cerr << "Error sending welcome message" << std::endl;
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
}

void Server::handelClientMessage(int clientSocket)
{
    Command::fillListOfCommands();
    char buffer[Server::BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    int messageSize = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (messageSize < 0)
    {
        std::cerr << "Error receiving message from client" << std::endl;
        close(clientSocket);
        this->clients.erase(clientSocket);
        exit(EXIT_FAILURE);
    }
    else
    {
        std::string message(buffer);
        Command command(message);
        command.execute();
    }
}

void Server::handleClientDisconnection(int clientSocket)
{

    std::cout << "Client disconnected" << std::endl;
    // Close client socket
    close(clientSocket);

    // Remove client from pollfds
    for (size_t i = 0; i < pollfds.size(); i++)
    {
        if (pollfds[i].fd == clientSocket)
        {
            pollfds.erase(pollfds.begin() + i);
            break;
        }
    }

    // Remove client from clients
    clients.erase(clientSocket);
}

bool isNumber(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}