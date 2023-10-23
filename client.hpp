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
    bool isValidPass;
    bool isInveted;
public:
    Client();
    Client(int socket, std::string password);
    Client(std::string nick, std::string user, int socket, std::string password);
    Client& operator=(const Client& other);
    Client(const Client& other);
    ~Client();

    std::string getNick();
    std::string getUser();
    int getSocket();
    std::string getPassword();
    bool getIsRegistered();
    bool getIsValidPass();

    void setNick(std::string nick);
    void setUser(std::string user);
    void setSocket(int socket);
    void setPassword(std::string password);
    void setIsRegistered(bool isRegistered);
    void setIsValidPass(bool isValidPass);

    void setInveted(bool isInveted);
    bool getInveted();
};

#endif // CLIENT_HPP