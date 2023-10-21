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
}

int Channel::checkNick(std::string nickname) {
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (this->clients[i].getNick() == nickname) {
            return i;
        }
    }
    return -1;
}