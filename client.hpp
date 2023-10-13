#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>

class client
{
private:
    std::string pwd;
    std::string user;
    std::string nick;
    int socket;
    std::string name;
    bool isPassword;
    bool isUser;
    bool isNick;
    bool isSocket;
    bool isName;
public:
    client();
    client(int socket, std::string pwd);
    client(const client& other);
    client& operator=(const client& other);
    ~client();
};

#endif