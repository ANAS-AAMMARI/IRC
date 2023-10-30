#include "channel.hpp"

Channel::Channel() {
    this->topic = "";
    this->name = "";
    this->topic_time = "";
    this->creation_time = "";
    this->pass_of_channel = "";
    this->limit = 0;
    this->is_encrypted = false;
    this->isInvited = false;
    this->topic_mode = true;
}

Channel::Channel(std::string name) {
    this->topic = "";
    this->topic_time = "";
    this->creation_time = "";
    this->name = name;
    this->pass_of_channel = "";
    this->limit = 0;
    this->is_encrypted = false;
    this->isInvited = false;
    this->topic_mode = true;
}

Channel& Channel::operator=(const Channel& other) {

    if (this == &other)
        return *this;
    this->topic = other.topic;
    this->name = other.name;
    this->pass_of_channel = other.pass_of_channel;
    this->is_encrypted = other.is_encrypted;
    this->limit = other.limit;
    this->isInvited = other.isInvited;
    this->topic_mode = other.topic_mode;
    this->clients = other.clients;
    this->listofAdmins = other.listofAdmins;
    this->listOfInvited = other.listOfInvited;
    this->creation_time = other.creation_time;
    this->topic_time = other.topic_time;
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

void Channel::setTopic(std::string topic) {
    this->topic = topic;
}

std::string Channel::getTopic() {
    return this->topic;
}

void Channel::setTopic_time(std::string topic_time) {
    this->topic_time = topic_time;
}

std::string Channel::getTopic_time() {
    return this->topic_time;
}

void Channel::setCreation_time(std::string creation_time) {
    this->creation_time = creation_time;
}

std::string Channel::getCreation_time() {
    return this->creation_time;
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

int Channel::getNumberOfClients() {
    return this->clients.size() + this->listofAdmins.size();
}

void Channel::addAdmin(Client &client) {
    this->listofAdmins.push_back(client);
}

void Channel::setPass(std::string pass) {
    this->pass_of_channel = pass;
}

std::string Channel::getPass() {
    return this->pass_of_channel;
}

void Channel::setEncrypted(bool is_encrypted) {
    this->is_encrypted = is_encrypted;
}

bool Channel::getEncrypted() {
    return this->is_encrypted;
}


void Channel::setLimit(int limit) {
    this->limit = limit;
}

int Channel::getLimit() {
    return this->limit;
}

void Channel::addoperator(std::string nickname) {
    for (size_t i = 0; i < this->clients.size(); i++) {
        if (this->clients[i].getNick() == nickname) {
            this->listofAdmins.push_back(this->clients[i]);
            this->clients.erase(this->clients.begin() + i);
            return;
        }
    }
}

void Channel::removeoperator(std::string nickname) {
    for (size_t i = 0; i < this->listofAdmins.size(); i++) {
        if (this->listofAdmins[i].getNick() == nickname) {
            this->clients.push_back(this->listofAdmins[i]);
            this->listofAdmins.erase(this->listofAdmins.begin() + i);
            return;
        }
    }
}

bool Channel::getInv_mode() {
    return this->isInvited;
}

void Channel::setInv_mode(bool inv_mode) {
    this->isInvited = inv_mode;
}

void Channel::addInvited(std::string nickname) {
    this->listOfInvited.push_back(nickname);
}

int Channel::checkInvited(std::string nickname) {
    for (size_t i = 0; i < this->listOfInvited.size(); i++) {
        if (this->listOfInvited[i] == nickname) {
            return i;
        }
    }
    return -1;
}

void Channel::setTopicMode(bool topic_mode) {
    this->topic_mode = topic_mode;
}

bool Channel::getTopicMode() {
    return this->topic_mode;
}