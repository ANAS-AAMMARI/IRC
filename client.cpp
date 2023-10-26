#include "client.hpp"

Client::Client() {
    this->nick = "";
    this->user = "";
    this->socket = -1;
    this->password = "";
    this->isRegistered = false;
    this->isValidPass = false;
}

Client::Client(int socket, std::string password) {
    this->socket = socket;
    this->password = password;
    this->user = "";
    this->nick = "";
    this->isRegistered = false;
    this->isValidPass = false;
}

Client::Client(std::string nick, std::string user, int socket, std::string password) {
    this->nick = nick;
    this->user = user;
    this->socket = socket;
    this->password = password;
    this->isRegistered = false;
    this->isValidPass = false;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        this->nick = other.nick;
        this->user = other.user;
        this->socket = other.socket;
        this->password = other.password;
        this->isRegistered = other.isRegistered;
        this->isValidPass = other.isValidPass;
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

bool Client::getIsValidPass() {
    return this->isValidPass;
}

bool Client::getIsRegistered() {
    return this->isRegistered;
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

void Client::setIsRegistered(bool isRegistered) {
    this->isRegistered = isRegistered;
}

void Client::setIsValidPass(bool isValidPass) {
    this->isValidPass = isValidPass;
}
