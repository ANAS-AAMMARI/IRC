#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <sstream>

#include "client.hpp"
#include "command.hpp"
#include "channel.hpp"

class Server {
private:
    long long port;
    std::string password;
    static const int BUFFER_SIZE;
    std::vector<struct pollfd> pollfds;
    std::map<int, Client> clients;
    std::map<int, Channel> channels;
    void handleNewConnection();
    void handelClientMessage(int clientSocket);
    void handleClientDisconnection(int clientSocket);

public:
    // Server();
    Server(long long port, std::string password);
    Server& operator=(const Server& other);
    Server(const Server& other);
    ~Server();
    void run();
    void setPort(long long port);
    std::vector<struct pollfd> getPollfds();
};

bool isNumber(std::string str);



#endif // SERVER_HPP