#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <netdb.h>
// #include <ctime>
// #include <cstdio>

#include "mode_opt.hpp"
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
    BOT,
};

class Channel;

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
    void execute(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    void trimString(std::string &str);
    int checkNickUser(std::map<int, Client> &client, std::string input, int who);
    void removeSpaces(std::string &msg);
    static void fillListOfCommands();
    void toUpper(std::string &str);
    void sendToClient(const std::string &msg, int clientSocket);
    std::string getCurrentDateTime();
    std::string getLoclalIp();
    void registerClient(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    void commandHandler(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);

    bool checkNick(const std::string &nick);
    int  check_if_exist(std::string chan, std::map<int, Channel> &channels);
    void PASSCommand(std::map<int, Client> &client, int index);
    void NICKCommand(std::map<int, Client> &client, int index);
    void USERCommand(std::map<int, Client> &client, int index);
    void PRIVMSGCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channel);
    void JOINCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    void INVITECommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    void PARTCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    void MODECommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    void TOPICCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    void KICKCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);


    void BOTCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels);
    int  get_ur_age(std::string &str);
};
// int checkUsrNick(std::map<int, Client> &client, int check, std::string str, int index);
// void check_channel(std::string channel, Client &client, std::map<int, Channel> &channels);

#endif // COMMAND_HPP