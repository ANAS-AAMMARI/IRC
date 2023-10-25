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
    std::string name;
    std::vector<Client> clients;
    std::vector<Client> listofAdmins;
    std::string pass_of_channel;
    bool is_encrypted;
    int limit;
    std::vector<std::string> listOfInvited;
    bool isInvited;
public:
    Channel();
    Channel(std::string name);
    Channel& operator=(const Channel& other);
    Channel(const Channel& other);
    ~Channel();
    void setName(std::string name);
    std::string getName();
    void addClient(Client &client);
    void addAdmin(Client &client);
    void addoperator(std::string nickname);
    void removeoperator(std::string nickname);
    void removeClient(std::string const nickname);
    void sendToAllButOne(std::string msg, std::string nickname);
    void sendToAll(std::string msg);
    int checkNick(std::string nickname);
    int checkAdmin(std::string nickname);
    std::string getClients();

    // option K
    void setPass(std::string pass);
    std::string getPass();
    void setEncrypted(bool is_encrypted);
    bool getEncrypted();

    // option L
    void setLimit(int limit);
    int  getLimit();

    int getNumberOfClients();

    void addInvited(std::string nickname);
    int checkInvited(std::string nickname);
    void    setInv_mode(bool inv_mode);
    bool    getInv_mode();
};

#endif