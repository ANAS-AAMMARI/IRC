#include "channel.hpp"

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

void Channel::addClient(Client client) {
    this->clients.push_back(client);
}

void Channel::removeClient(int index) {
    this->clients.erase(this->clients.begin() + index);
}

void Channel::sendToAll(std::string msg) {
    for (int i = 0; i < this->clients.size(); i++) {
        send(this->clients[i].getSocket(), msg.c_str(), msg.size(), 0);
    }
}

void Channel::sendToAllButOne(std::string msg, int index) {
    for (int i = 0; i < this->clients.size(); i++) {
        if (i != index) {
            send(this->clients[i].getSocket(), msg.c_str(), msg.size(), 0);
        }
    }
}
