#include "command.hpp"

std::vector<std::string> Command::listOfCommands;

void Command::sendToClient(const std::string &msg, int clientSocket)
{
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

void Command::fillListOfCommands()
{
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

Command::Command(std::string msg, std::map<int, Client> &client, int index)
{
    this->msg = msg;
    this->command = "";
    this->indexOfCommand = -1;
    this->parse(client[index]);
}

Command &Command::operator=(const Command &other)
{
    if (this != &other)
    {
        this->msg = other.msg;
        this->command = other.command;
        this->indexOfCommand = other.indexOfCommand;
        this->args = other.args;
    }
    return *this;
}

Command::Command(const Command &other)
{
    *this = other;
}

Command::~Command()
{
}

std::string Command::getCommand()
{
    return this->command;
}

std::vector<std::string> Command::getArgs()
{
    return this->args;
}

std::string Command::getMsg()
{
    return this->msg;
}

void Command::setMsg(std::string msg)
{
    this->msg = msg;
}

int Command::getIndexOfCommand()
{
    return this->indexOfCommand;
}

void Command::setIndexOfCommand(int index)
{
    this->indexOfCommand = index;
}

std::string Command::getLoclalIp()
{
    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        return "NULL";
    }
    
    struct hostent *host = gethostbyname(hostname);
    if (host == NULL) {
        return "NULL";
    }
    
    char* ipAddress = inet_ntoa(*(struct in_addr *)host->h_addr);
    if (ipAddress == NULL) {
        return "NULL";
    }
    
    return std::string(ipAddress);
}

void Command::trimString(std::string &str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start != std::string::npos)
        str = str.substr(start);

    size_t end = str.find_last_not_of(" \t\n\r");
    if (end != std::string::npos)
        str = str.substr(0, end + 1);
}

std::string Command::removeSpaces(std::string &msg)
{
    trimString(msg);
    std::string result;
    bool previousIsSpace = false;
    for (size_t i = 0; i < msg.size(); i++) {
        if (std::isspace(msg[i])) {
            if (!previousIsSpace) {
                result += ' ';
            }
            previousIsSpace = true;
        } else {
            result += msg[i];
            previousIsSpace = false;
        }
    }
    return result;
}

void Command::toUpper(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void Command::parse(Client &client)
{
    std::string temp;
    this->removeSpaces(this->msg);
    std::stringstream ss(this->msg);
    std::getline(ss, temp, ' ');
    this->toUpper(temp);
    for (size_t i = 0; i < Command::listOfCommands.size(); i++)
    {
        if (temp == Command::listOfCommands[i])
        {
            this->command = temp;
            this->indexOfCommand = i;
            break;
        }
    }
    if (this->indexOfCommand == -1 && client.getIsRegistered())
    {
        sendToClient(UNKNOW_COMMAND_MSG(client.getNick(), this->command), client.getSocket());
        return;
    }
    if (this->msg.find(':') != std::string::npos)
    {
        std::getline(ss, temp, ':');
        // this->trimString(temp);
        std::stringstream ss2(temp);
        while (std::getline(ss2, temp, ' '))
        {
            // this->trimString(temp);
            this->args.push_back(temp);
        }
        std::getline(ss, temp);
        // this->trimString(temp);
        this->args.push_back(temp);
    }
    else
    {
        while (std::getline(ss, temp, ' '))
        {
            // this->trimString(temp);
            this->args.push_back(temp);
        }
    }
}

std::string Command::getCurrentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %b %d %Y at %H:%M:%S %Z", &tstruct);

    return std::string(buf);
}

bool Command::checkNick(const std::string &nickname)
{
    if (nickname.empty() || nickname[0] == ':' || nickname.find(' ') != std::string::npos)
    {
        return false;
    }
    const std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]{}\\`^|";

    if (isdigit(nickname[0]))
        return false;
    if (nickname.find_first_not_of(validChars) != std::string::npos)
        return false;
    return true;
}

int Command::checkNickUser(std::map<int, Client> &client, std::string input, int who)
{
    if (input.empty())
        return -1;
    if (who == 1)
    {
        for (std::map<int, Client>::iterator it = client.begin(); it != client.end(); it++)
        {
            if (it->second.getNick() == input)
                return it->first;
        }
    }
    else if (who == 2)
    {
        for (std::map<int, Client>::iterator it = client.begin(); it != client.end(); it++)
        {
            if (it->second.getUser() == input)
                return it->first;
        }
    }
    return -1;
}

void Command::PASSCommand(std::map<int, Client> &client, int index)
{
    if (client[index].getIsRegistered())
    {
        sendToClient(ALREADY_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 1)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "PASS"), client[index].getSocket());
        return;
    }
    if (args[0] != client[index].getPassword())
    {
        sendToClient(PASS_INC_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    client[index].setIsValidPass(true);
}

void Command::NICKCommand(std::map<int, Client> &client, int index)
{
    if (this->args.size() < 1)
    {
        sendToClient(NICK_REQUIRED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (!checkNick(this->args[0]))
    {
        sendToClient(NICK_INVALID_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args[0] == client[index].getNick())
        return;
    if (checkNickUser(client, this->args[0], 1) != -1)
    {
        if (client[index].getIsRegistered())
            sendToClient(NICK_ALREADY_MSG(client[index].getNick()), client[index].getSocket());
        else
            sendToClient(NICK_ALREADY_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    std::string oldNick = client[index].getNick();
    client[index].setNick(this->args[0]);
    if (client[index].getIsRegistered())
        sendToClient(NICK_MSG(oldNick, client[index].getNick(), client[index].getUser(), getLoclalIp()), client[index].getSocket());
}

void Command::USERCommand(std::map<int, Client> &client, int index)
{
    if (client[index].getIsRegistered())
    {
        sendToClient(ALREADY_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 4)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "USER"), client[index].getSocket());
        return;
    }
    if (checkNickUser(client, this->args[0], 2) != -1 && client[index].getIsRegistered())
    {
        sendToClient(ALREADY_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    client[index].setUser(this->args[0]);
}

void Command::registerClient(std::map<int, Client> &client, int index, std::map<int, Channel> &channel)
{
    switch (this->indexOfCommand)
    {
    case PASS:
        this->PASSCommand(client, index);
        break;
    case NICK:
        this->NICKCommand(client, index);
        break;
    case USER:
        this->USERCommand(client, index);
        break;
    case PRIVMSG:
        this->PRIVMSGCommand(client, index, channel);
        break;
    default:
        break;
    }
    if (!client[index].getIsValidPass())
        return;
    if (client[index].getNick().empty() || client[index].getUser().empty())
        return;
    client[index].setIsRegistered(true);
    sendToClient(WELCOME_MSG(client[index].getNick(), getLoclalIp(), client[index].getUser()), client[index].getSocket());
    sendToClient(YOURHOST_MSG(client[index].getNick(), getLoclalIp()), client[index].getSocket());
    sendToClient(CREATED_MSG(client[index].getNick(), getCurrentDateTime()), client[index].getSocket());
    sendToClient(MYINFO_MSG(client[index].getNick(), "+", "t"), client[index].getSocket());
    sendToClient(ISUPPORT_MSG(client[index].getNick(), "PREFIX=(ov)@+ CHANTYPES=#&+ CHANMODES=,,,"), client[index].getSocket());
}

void Command::PRIVMSGCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channel)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(PRIVMSG_NOTREGISTERED(client[index].getNick()), client[index].getSocket());
        return;
    }
   
    if (this->args.size() < 2)
    {
        if (this->args.size() < 1)
            sendToClient(PRIVMSG_NORECIPIENT_MSG(client[index].getNick()), client[index].getSocket());
        else
            sendToClient(PRIVMSG_NOTEXTTOSEND_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args[0][0] == '#')
    {
        int id = check_if_exist(this->args[0], channel);
        if (channel.find(id) != channel.end())
        {
            if (channel[id].checkNick(client[index].getNick()) == -1)
            {
                sendToClient(PRIVMSG_CANNOTSENDTOCHAN_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
                return;
            }
            channel[id].sendToAllButOne(PRIVMSG_AWAY_MSG(client[index].getNick(),
                client[index].getUser(), getLoclalIp(), this->args[0], this->args[1]), client[index].getNick());
        }    
        else
           sendToClient(PRIVMSG_NOSUCHNICK_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
    }
    else
    {
        int id = checkNickUser(client, this->args[0], 1);
        if (id != -1)
            sendToClient(PRIVMSG_AWAY_MSG(client[index].getNick(),
                client[index].getUser(), getLoclalIp(), this->args[0], this->args[1]), client[id].getSocket());
        else
            sendToClient(PRIVMSG_NOSUCHNICK_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
    }
}

void Command::execute(std::map<int, Client> &client, int index, std::map<int, Channel> &channel)
{
    if (!client[index].getIsRegistered())
        this->registerClient(client, index, channel);
    else
    {
        switch (this->indexOfCommand)
        {
        case PASS:
            this->PASSCommand(client, index);
            break;
        case NICK:
            this->NICKCommand(client, index);
            break;
        case USER:
            this->USERCommand(client, index);
            break;
        case PRIVMSG:
            this->PRIVMSGCommand(client, index, channel);
            break;
        case JOIN:
            break;
        case PART:
            break;
        case MODE:
            break;
        case TOPIC:
            break;
        case KICK:
            break;
        case INVITE:
            break;
        default:
            break;
        }
    }
}

int Command::check_if_exist(std::string channel, std::map<int, Channel> &channels)
{
    std::map<int, Channel>::iterator it;
    int i = 0;
    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second.getName() == channel)
            return i;
        i++;
    }
    return -1;
}