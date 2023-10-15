#include "command.hpp"

std::vector<std::string> Command::listOfCommands;

void Command::fillListOfCommands() {
    Command::listOfCommands.push_back("PASS");
    Command::listOfCommands.push_back("NICK");
    Command::listOfCommands.push_back("USER");
    Command::listOfCommands.push_back("PRIVMSG");
    Command::listOfCommands.push_back("JOIN");
    Command::listOfCommands.push_back("PART");
    Command::listOfCommands.push_back("MODE");
    Command::listOfCommands.push_back("TOPIC");
    Command::listOfCommands.push_back("KICK");
    Command::listOfCommands.push_back("INVITE");
}

Command::Command(std::string msg, Client &client) {
    this->msg = msg;
    this->command = "";
    this->indexOfCommand = -1;
    this->parse(client.getSocket());
}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        this->msg = other.msg;
        this->command = other.command;
        this->indexOfCommand = other.indexOfCommand;
        this->args = other.args;
    }
    return *this;
}

Command::Command(const Command& other) {
    *this = other;
}

Command::~Command() {
}

std::string Command::getCommand() {
    return this->command;
}

std::vector<std::string> Command::getArgs() {
    return this->args;
}

std::string Command::getMsg() {
    return this->msg;
}

void Command::setMsg(std::string msg) {
    this->msg = msg;
}

int Command::getIndexOfCommand() {
    return this->indexOfCommand;
}

void Command::setIndexOfCommand(int index) {
    this->indexOfCommand = index;
}

void Command::trimString(std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start != std::string::npos) {
        str = str.substr(start);
    }
    
    size_t end = str.find_last_not_of(" \t\n\r");
    if (end != std::string::npos) {
        str = str.substr(0, end + 1);
    }
}

void Command::toUpper(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void Command::parse(int socket)
{
    std::string temp;
    this->trimString(this->msg);
    std::stringstream ss(this->msg);
    std::getline(ss, temp, ' ');
    this->toUpper(temp);

    //std::cout << temp << std::endl;

    for(size_t i = 0; i < Command::listOfCommands.size(); i++) {
        if (temp == Command::listOfCommands[i]) {
            this->command = temp;
            this->indexOfCommand = i;
            break;
        }
    }
    if (this->indexOfCommand == -1) {
        std::string message = "Invalid command\n";
        send(socket, message.c_str(), message.size(), 0);
        return;
    }
    if (this->msg.find(':') != std::string::npos) {
        /*std::getline(ss, temp, ':');
        this->trimString(temp);
        std::stringstream ss2(temp);
        while (std::getline(ss2, temp, ' ')) {
            this->trimString(temp);
            this->args.push_back(temp);
        }
        std::getline(ss, temp);
        this->trimString(temp);
        this->args.push_back(temp);*/
    std::istringstream ss(this->msg);
    std::string temp;
    std::getline(ss, temp, ':');
    while (std::getline(ss, temp, ':')) {
        this->trimString(temp); // Assuming trimString is a valid function to remove extra spaces
        std::istringstream ss2(temp);
        std::string word;
        while (std::getline(ss2, word, ' ')) {
            this->trimString(word); // Assuming trimString is a valid function to remove extra spaces
            this->args.push_back(word);
            }
        }
    }
}

void Command::Password(Client &client) {
    std::string message;
    if (client.getIsRegistered() == true)
    {
        message = "You are already registered and logged in\n";
        send(client.getSocket(), message.c_str(), message.size(), 0);
        return;
    }
    if (args.size() != 1) {
        message = "Wrong number of arguments(invalid password)\n";
        send(client.getSocket(), message.c_str(), message.size(), 0);
        return;
    }
    if (args[0] != client.getPassword()) {
        message = "Wrong password\n";
        send(client.getSocket(), message.c_str(), message.size(), 0);
        return;
    }
    if (args[0] == client.getPassword()) {
        message = "You are now registered and logged in, Welcomee\n";
        send(client.getSocket(), message.c_str(), message.size(), 0);
        client.setIsRegistered(true);
    }
}

void Command::Nick() {
    std::cout << "Nick" << std::endl;
}

void Command::User() {
    std::cout << "User" << std::endl;
}


void Command::execute(Client &client)
{
    switch (this->indexOfCommand) {
        case 0:
            this->Password(client);
            break;
        case 1:
            this->Nick();
            break;
        case 2:
            this->User();
            break;
        default:
            break; 
    }
    /*void (Command::*commands[])() = {&Command::Password, &Command::Nick, &Command::User};
    if (this->indexOfCommand != -1)
        (this->*commands[this->indexOfCommand])();*/
}