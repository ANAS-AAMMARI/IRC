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
    bool isRegisteredPWD;
    bool isRegisteredUSER;
    bool isRegistered;
    int  check;
    int  cmdcheck;
    //std::vector<Channel> channels;
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
    int getCheck();

    bool getIsRegistered();
    bool getIsRegisteredPWD();
    bool getIsRegisteredUSER();
    int getcmdcheck();

    void setIsRegisteredPWD(bool isRegisteredPWD);
    void setIsRegisteredUSER(bool isRegisteredUSER);
    void setIsRegistered(bool isRegistered);
    void increaseCheck();

    void setNick(std::string nick);
    void setUser(std::string user);
    void setSocket(int socket);
    void setPassword(std::string password);
    void setcmdcheck(int check);

    //void    joinChannel(std::string channel);
};

#endif // CLIENT_HPP