#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

class Client {
private:
    std::string nick;
    std::string user;
    int socket;
    std::string password;
    bool isRegistered;

public:
    Client(std::string nick, std::string user, int socket, std::string password);
    Client& operator=(const Client& other);
    Client(const Client& other);
    ~Client();
    std::string getNick();
    std::string getUser();
    int getSocket();
    std::string getPassword();
    void setNick(std::string nick);
    void setUser(std::string user);
    void setSocket(int socket);
    void setPassword(std::string password);
};




#endif // CLIENT_HPP