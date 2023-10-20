#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <netdb.h>

#include "server.hpp"
#include "client.hpp"
#include "tools.hpp"
#include "channel.hpp"

enum Commands{
    PASS,
    NICK,
    USER,
    PRIVMSG,
    JOIN,
    PART,
    MODE,
    TOPIC,
    KICK,
    INVITE,
};

class Command {
private:
    std::string msg;
    std::string command;
    int indexOfCommand;
    std::vector<std::string> args;
    static Channel *channel;
public:
    static std::vector<std::string> listOfCommands;
    Command(std::string msg, std::map<int, Client> &client, int index);
    Command& operator=(const Command& other);
    Command(const Command& other);
    ~Command();

    std::string getCommand();
    std::vector<std::string> getArgs();
    std::string getMsg();
    void setMsg(std::string msg);
    int getIndexOfCommand();
    void setIndexOfCommand(int index);

    void parse(Client &client);
    void execute(std::map<int, Client> &client, int index);
    void trimString(std::string &str);
    static void fillListOfCommands();
    void toUpper(std::string &str);
    void sendToClient(const std::string &msg, int clientSocket);

    void Password(std::map<int, Client> &client, int index);
    void Nick(std::map<int, Client> &client, int index);
    void User(std::map<int, Client> &client, int index);
    void Privmsg(std::map<int, Client> &client, int index);
    void Join(std::map<int, Client> &client, int index);
    int checkUsrNick(std::map<int, Client> &client, int check, std::string str, int index);
};

#endif // COMMAND_HPP