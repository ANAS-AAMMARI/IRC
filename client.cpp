#include "client.hpp"

Client::Client() {
    this->isRegistered = false;
    this->isRegisteredPWD = false;
    this->isRegisteredUSER = false;
    this->nick = "";
    this->user = "";
    this->check = 0;
}

int Client::getCheck() {
    return this->check;
}

void Client::increaseCheck() {
    this->check++;
}
bool Client::getIsRegistered() {
    return this->isRegistered;
}

bool Client::getIsRegisteredPWD() {
    return this->isRegisteredPWD;
}

bool Client::getIsRegisteredUSER() {
    return this->isRegisteredUSER;
}

void Client::setIsRegisteredPWD(bool isRegisteredPWD) {
    this->isRegisteredPWD = isRegisteredPWD;
}

void Client::setIsRegisteredUSER(bool isRegisteredUSER) {
    this->isRegisteredUSER = isRegisteredUSER;
}

void Client::setIsRegistered(bool isRegistered) {
    this->isRegistered = isRegistered;
}

Client::Client(int socket, std::string password) {
    this->socket = socket;
    this->password = password;
    this->isRegistered = false;
}

Client::Client(std::string nick, std::string user, int socket, std::string password) {
    this->nick = nick;
    this->user = user;
    this->socket = socket;
    this->password = password;
    this->isRegistered = false;
    this->isRegisteredPWD = false;
    this->isRegisteredUSER = false;
    this->check = 0;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        this->nick = other.nick;
        this->user = other.user;
        this->socket = other.socket;
        this->password = other.password;
        this->isRegistered = other.isRegistered;
        this->isRegisteredPWD = other.isRegisteredPWD;
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