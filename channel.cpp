#include "channel.hpp"

Channel::Channel() {
    this->name = "";
    this->topic = "";
}

Channel::Channel(std::string name) {
    this->name = name;
    this->topic = "";
}

Channel& Channel::operator=(const Channel& other) {
    if (this != &other) {
        this->name = other.name;
        this->topic = other.topic;
        this->clients = other.clients;
        this->listofAdmins = other.listofAdmins;
    }
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

void Channel::setTopic(std::string topic) {
    this->topic = topic;
}

std::string Channel::getName() {
    return this->name;
}

std::string Channel::getTopic() {
    return this->topic;
}

void Channel::addClient(Client &client) {
    this->clients.push_back(client);
}

void Channel::removeClient(std::string const nickname) {
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (this->clients[i].getNick() == nickname) {
            this->clients.erase(this->clients.begin() + i);
            return;
        }
    }
    for (size_t i = 0; i < this->listofAdmins.size(); i++) {
        if (this->listofAdmins[i].getNick() == nickname) {
            this->listofAdmins.erase(this->listofAdmins.begin() + i);
            return;
        }
    }
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