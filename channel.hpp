#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "server.hpp"
#include "client.hpp"

class Client;

class Channel {
private:
    std::string topic;
    std::string name;
    std::vector<Client> clients;
    std::vector<Client> listofAdmins;
public:
    Channel();
    Channel(std::string name);
    Channel& operator=(const Channel& other);
    Channel(const Channel& other);
    ~Channel();
    void setName(std::string name);
    void setTopic(std::string topic);
    std::string getName();
    std::string getTopic();
    void addClient(Client &client);
    void addAdmin(Client &client);
    void removeClient(std::string const nickname);
    void sendToAllButOne(std::string msg, std::string nickname);
    void sendToAll(std::string msg);
    int checkNick(std::string nickname);
    int checkAdmin(std::string nickname);
    std::string getClients();
};

#endif