#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "server.hpp"
#include "client.hpp"

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
public:
    static std::vector<std::string> listOfCommands;
    Command(std::string msg, Client &client);
    Command& operator=(const Command& other);
    Command(const Command& other);
    ~Command();
    std::string getCommand();
    std::vector<std::string> getArgs();
    std::string getMsg();
    void setMsg(std::string msg);
    int getIndexOfCommand();
    void setIndexOfCommand(int index);
    void parse(int socket);
    void execute(Client &client);
    void trimString(std::string &str);
    static void fillListOfCommands();
    void toUpper(std::string &str);
    // void sendToClient(std::string msg, int clientSocket);
    void Password(Client &client);
    void Nick();
    void User();

};

#endif // COMMAND_HPP