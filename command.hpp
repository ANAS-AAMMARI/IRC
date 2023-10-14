#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

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
    Command(std::string msg);
    Command& operator=(const Command& other);
    Command(const Command& other);
    ~Command();
    std::string getCommand();
    std::vector<std::string> getArgs();
    std::string getMsg();
    void setMsg(std::string msg);
    int getIndexOfCommand();
    void setIndexOfCommand(int index);
    void parse();
    void execute();
    void trimString(std::string &str);
    static void fillListOfCommands();
    void toUpper(std::string &str);
    // void sendToClient(std::string msg, int clientSocket);
    void Password();
    void Nick();
    void User();

};

#endif // COMMAND_HPP