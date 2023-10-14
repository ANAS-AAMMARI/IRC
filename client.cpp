#include "client.hpp"

Client::Client(std::string nick, std::string user, int socket, std::string password) {
    this->nick = nick;
    this->user = user;
    this->socket = socket;
    this->password = password;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        this->nick = other.nick;
        this->user = other.user;
        this->socket = other.socket;
        this->password = other.password;
    }
    return *this;
}

Client::Client(const Client& other) {
    *this = other;
}

Client::~Client() {
}

std::string Client::getNick() {
    return this->nick;
}

std::string Client::getUser() {
    return this->user;
}

int Client::getSocket() {
    return this->socket;
}

std::string Client::getPassword() {
    return this->password;
}

void Client::setNick(std::string nick) {
    this->nick = nick;
}

void Client::setUser(std::string user) {
    this->user = user;
}

void Client::setSocket(int socket) {
    this->socket = socket;
}

void Client::setPassword(std::string password) {
    this->password = password;
}
