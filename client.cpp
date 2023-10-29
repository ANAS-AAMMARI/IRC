#include "client.hpp"

Client::Client() {
    this->nick = "";
    this->user = "";
    this->socket = -1;
    this->password = "";
    this->isRegistered = false;
    this->isValidPass = false;
    this->Random_number = 0;
    this->tryed = -1;
    this->guess_number = -1;
    this->game_start = 0;
}

Client::Client(int socket, std::string password) {
    this->socket = socket;
    this->password = password;
    this->user = "";
    this->nick = "";
    this->isRegistered = false;
    this->isValidPass = false;
    this->Random_number = 0;
    this->tryed = 0;
    this->guess_number = -1;
    this->game_start = 0;
}

Client::Client(std::string nick, std::string user, int socket, std::string password) {
    this->nick = nick;
    this->user = user;
    this->socket = socket;
    this->password = password;
    this->isRegistered = false;
    this->isValidPass = false;
    this->Random_number = 0;
    this->tryed = 0;
    this->guess_number = -1;
    this->game_start = 0;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {
        this->nick = other.nick;
        this->user = other.user;
        this->socket = other.socket;
        this->password = other.password;
        this->isRegistered = other.isRegistered;
        this->isValidPass = other.isValidPass;
        this->Random_number = other.Random_number;
        this->tryed = other.tryed;
        this->guess_number = other.guess_number;
        this->game_start = other.game_start;
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

void Client::setGameStart(int game_start) {
    this->game_start = game_start;
}

void Client::setTryed(int tryed) {
    this->tryed = tryed;
}

void Client::increaseTryed() {
    this->tryed++;
}

void Client::setRandomNumber(int Random_number) {
    this->Random_number = Random_number;
}

void Client::setguess_number(int guess_number) {
    this->guess_number = guess_number;
}

int Client::getGameStart() {
    return this->game_start;
}

int Client::getTryed() {
    return this->tryed;
}

int Client::getRandomNumber() {
    return this->Random_number;
}

int Client::getguess_number() {
    return this->guess_number;
}