#include "channel.hpp"

Channel::Channel() {
    this->name = "";
}

Channel::Channel(std::string name) {
    this->name = name;
}

Channel& Channel::operator=(const Channel& other) {
    this->name = other.name;
    return *this;
}

Channel::Channel(const Channel& other) {
    *this = other;
}

Channel::~Channel() {
}

void Channel::setName(std::string name) {
    this->name = name;
}

std::string Channel::getName() {
    return this->name;
}

void Channel::addClient(Client &client) {
    this->clients.push_back(client);
}

void Channel::removeClient(int index) {
    this->clients.erase(this->clients.begin() + index);
}

void Channel::sendToAllButOne(std::string msg, std::string nickname) {
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (this->clients[i].getNick() != nickname) {
            send(this->clients[i].getSocket(), msg.c_str(), msg.size(), 0);
        }
    }
    for (size_t i = 0; i < this->listofAdmins.size(); i++) {
        if (this->listofAdmins[i].getNick() != nickname) {
            send(this->listofAdmins[i].getSocket(), msg.c_str(), msg.size(), 0);
        }
    }
}

int Channel::checkAdmin(std::string nickname) {
    for (size_t i = 0; i < this->listofAdmins.size(); i++) {
        if (this->listofAdmins[i].getNick() == nickname) {
            return i;
        }
    }
    return -1;
}

int Channel::checkNick(std::string nickname) {
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (this->clients[i].getNick() == nickname) {
            return i;
        }
    }
    for (size_t i = 0; i < this->listofAdmins.size(); i++) {
        if (this->listofAdmins[i].getNick() == nickname) {
            return i;
        }
    }
    return -1;
}

void Channel::sendToAll(std::string msg) {
    for (size_t i = 0; i < this->clients.size(); i++) {
        send(this->clients[i].getSocket(), msg.c_str(), msg.size(), 0);
    }
    for (size_t i = 0; i < this->listofAdmins.size(); i++) {
        send(this->listofAdmins[i].getSocket(), msg.c_str(), msg.size(), 0);
    }
}

std::string Channel::getClients() {
    std::string clients;
    for (size_t i = 0; i < this->clients.size(); i++) {
        clients += this->clients[i].getNick() + " ";
    }
    for (size_t i = 0; i < this->listofAdmins.size(); i++) {
        clients += "@" + this->listofAdmins[i].getNick() + " ";
    }
    return clients;
}

void Channel::addAdmin(Client &client) {
    this->listofAdmins.push_back(client);
}