#include "command.hpp"

std::vector<std::string> Command::listOfCommands;

int Command::hostnameToIP(std::string hostname) {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ((he = gethostbyname(hostname.c_str())) == NULL) {
        return -1;
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    for (i = 0; addr_list[i] != NULL; i++) {
        return addr_list[i]->s_addr;
    }
    return -1;
}

void    Command::sendToClient(const std::string &msg, int clientSocket) {
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

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

Command::Command(std::string msg, std::map<int, Client> &client, int index) {
    this->msg = msg;
    this->command = "";
    this->indexOfCommand = -1;
    this->parse(client[index]);
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

void Command::parse(Client &client)
{
    std::string temp;
    this->trimString(this->msg);
    std::stringstream ss(this->msg);
    std::getline(ss, temp, ' ');
    this->toUpper(temp);
    for(size_t i = 0; i < Command::listOfCommands.size(); i++) {
        if (temp == Command::listOfCommands[i]) {
            this->command = temp;
            this->indexOfCommand = i;
            break;
        }
    }
    if (this->indexOfCommand == -1 && client.getCheck() >= 3) {
        sendToClient(UNKNOWN_COMMAND_MSG(this->msg), client.getSocket());
        return;
    }
    if (this->msg.find(':') != std::string::npos) {
        std::getline(ss, temp, ':');
        this->trimString(temp);
        std::stringstream ss2(temp);
        while (std::getline(ss2, temp, ' ')) {
            this->trimString(temp);
            this->args.push_back(temp);
        }
        std::getline(ss, temp);
        this->trimString(temp);
        this->args.push_back(temp);
    }
    else{
        while (std::getline(ss, temp, ' ')) {
            this->trimString(temp);
            this->args.push_back(temp);
        }
    }
}

void    Command::Privmsg(std::map<int, Client> &client, int index) {
    if (args.size() < 0) {
        sendToClient("Wrong number of arguments\n", client[index].getSocket());
        return;
    }
    int j = 0;
    if (args.size() >= 1)
        int j = checkUsrNick(client, 3, args[0], index);
    if (j != 0) {
        std::string message = "You received this message: " + args[1] + "from , " + client[index].getNick() + "\n";
        sendToClient(message, j);
        return;
    }
    //std::cout << "Here is the message: " << args[1] << std::endl;
    //sendToClient("PRIVMSG " + args[0] + " :" + args[1] + "\n", client[index].getSocket());
}

void Command::Password(std::map<int, Client> &client, int index) {
    if (client[index].getIsRegisteredPWD() == true)
    {
        if (client[index].getCheck() >= 3) {
            sendToClient("You are already registered\n", client[index].getSocket());
            return;
        }
        sendToClient(ALREADY_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (args.size() != 1) {
        sendToClient("Wrong number of arguments\n", client[index].getSocket());
        return;
    }
    if (args[0] != client[index].getPassword()) {
        sendToClient(PASS_INC_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (args[0] == client[index].getPassword()) {
        sendToClient("Password accepted\n", client[index].getSocket());
        client[index].setIsRegisteredPWD(true);
        client[index].increaseCheck();
    }
}

void Command::Nick(std::map<int, Client> &client, int index) {
    if (client[index].getIsRegisteredPWD() == false){
        sendToClient("Password not registered\n", client[index].getSocket());
        return;
    }
    if (args.size() < 1) {
        sendToClient(NICK_REQUIRED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (!checkUsrNick(client, 1, args[0], index)) {
        sendToClient(NICK_ALREADY_MSG(args[0]), client[index].getSocket());
        return;
    }
    client[index].setNick(args[0]);
    client[index].increaseCheck();
    if (client[index].getCheck() == 3)
        sendToClient(WELCOME_MSG(args[0], "www.irc") , client[index].getSocket());
}

void Command::User(std::map<int, Client> &client, int index) {
    if (client[index].getIsRegisteredPWD() == false){
        sendToClient("Password not registered\n", client[index].getSocket());
        return;
    }
    if (client[index].getIsRegisteredUSER()) {
        if (client[index].getCheck() >= 3) {
            sendToClient("You are already registered\n", client[index].getSocket());
            return;
        }
        sendToClient("User already registered\n", client[index].getSocket());
        return;
    }
    if (args.size() < 4) {
        sendToClient("Wrong number of arguments ( you need at least 4)\n", client[index].getSocket());
        return;
    }
    /*if (!checkUsrNick(client, 2, args[0], index)) {
        sendToClient("Username already registered, (used by another client)\n", client[index].getSocket());
        return;
    }*/
    client[index].setUser(args[0]);
    client[index].setIsRegisteredUSER(true);
    client[index].increaseCheck();
    if (client[index].getCheck() == 3)
        sendToClient("Welcone to IRC server " + client[index].getNick() + "\n", client[index].getSocket());
}


void Command::execute(std::map<int, Client> &client, int index)
{
    switch (this->indexOfCommand) {
        case 0:
            this->Password(client, index);
            break;
        case 1:
            this->Nick(client, index);
            break;
        case 2:
            this->User(client, index);
            break;
        case 3:
            this->Privmsg(client, index);
        default:
            break; 
    }
}

int Command::checkUsrNick(std::map<int , Client> &client, int check, std::string str, int index)
{
    if (check == 1)
    {
        std::map<int, Client>::iterator it;
        for (it = client.begin(); it != client.end(); ++it)
        {
            if (it->second.getNick() == str)
                return 0;
        }
    }
    /*if (check == 2)
    {
        std::map<int, Client>::iterator it;
        for (it = client.begin(); it != client.end(); ++it)
        {
            if (it->second.getUser() == str)
                return 0;
        }
    }*/
    if (check == 3)
    {
        std::map<int, Client>::iterator it;
        int i = 4;
        for (it = client.begin(); it != client.end(); ++it)
        {
            if (i == index)
            {
                i++;
                continue;
            }
            if (it->second.getNick() == str)
                return (i);
            i++;
        }
        return (0);
    }
    return 1;
}


// parse function args part
/*std::istringstream ss(this->msg);
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
        }*/