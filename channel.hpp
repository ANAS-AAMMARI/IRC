#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "client.hpp"
#include "tools.hpp"
#include "server.hpp"
#include "command.hpp"

class Channel {
private:
    std::string name;
    std::vector<Client> clients;
public:
    Channel(std::string name);
    Channel& operator=(const Channel& other);
    Channel(const Channel& other);
    ~Channel();
    void setName(std::string name);
    std::string getName();
    void addClient(Client client);
    void removeClient(int index);
    void sendToAll(std::string msg);
    void sendToAllButOne(std::string msg, int index);
};

#endif