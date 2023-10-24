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

void Command::removeSpaces(std::string &msg)
{
    std::string temp = "";
    trimString(msg);
    size_t index = msg.find(":");
    if (index != std::string::npos)
        temp = msg.substr(0, index);
    else
    {
        temp = msg;
        index  = msg.size();
    }
    trimString(temp);
    std::string result;
    bool previousIsSpace = false;
    for (size_t i = 0; i < temp.size(); i++) {
        if (std::isspace(temp[i])) {
            if (!previousIsSpace) {
                result += ' ';
            }
            previousIsSpace = true;
        } else {
            result += temp[i];
            previousIsSpace = false;
        }
    }
   this->msg = result + msg.substr(index);
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
        std::stringstream ss2(temp);
        while (std::getline(ss2, temp, ' '))
            this->args.push_back(temp);
        std::getline(ss, temp);
        this->args.push_back(temp);
    }
    else
        while (std::getline(ss, temp, ' '))
            this->args.push_back(temp);
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

void Command::registerClient(std::map<int, Client> &client, int index, std::map<int, Channel> &channel)
{
    this->commandHandler(client, index, channel);
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

// PASS Command ***********************************************************
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

// NICK Command ***********************************************************
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

// USER Command ***********************************************************
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

// PRIVMSG Command ***********************************************************
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
    std::stringstream ss(this->args[0]);
    std::string getline;
    std::vector<std::string> recipients;
    while (std::getline(ss, getline, ','))
        recipients.push_back(getline);
    for (size_t i = 0; i < recipients.size(); i++)
    {
        if (recipients[i].empty())
            continue;
        if (recipients[i][0] == '#')
        {
            int id = check_if_exist(recipients[i], channel);
            if (channel.find(id) != channel.end())
            {
                if (channel[id].checkNick(client[index].getNick()) == -1)
                {
                    sendToClient(PRIVMSG_CANNOTSENDTOCHAN_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
                    return;
                }
                channel[id].sendToAllButOne(PRIVMSG_AWAY_MSG(client[index].getNick(),
                    client[index].getUser(), getLoclalIp(), this->args[0], this->args[1]), client[index].getNick());
            }    
            else
                sendToClient(PRIVMSG_NOSUCHNICK_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
        }
        else
        {
            int id = checkNickUser(client, recipients[i], 1);
            if (id != -1)
                sendToClient(PRIVMSG_AWAY_MSG(client[index].getNick(),
                    client[index].getUser(), getLoclalIp(), recipients[i], this->args[1]), client[id].getSocket());
            else
                sendToClient(PRIVMSG_NOSUCHNICK_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
        }
    }
}

// JOIN Command ****************************************************************
void Command::JOINCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(JOIN_NOTREGISTERED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 1)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "JOIN"), client[index].getSocket());
        return;
    }
    std::stringstream ss(this->args[0]);
    std::string getline;
    std::vector<std::string> recipients;
    while (std::getline(ss, getline, ','))
        recipients.push_back(getline);
    for (size_t i = 0; i < recipients.size(); i++)
    {
        if (recipients[i].empty())
            continue;
         if (recipients[i][0] != '#')
        {
            sendToClient(JOIN_NO_SUCH_CHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
            continue;
        }
        int id = check_if_exist(recipients[i], channels);
        if (id != -1)
        {
            // this part need error msg *********************************************************
            if (channels[id].checkNick(client[index].getNick()) != -1)
                continue;
            if (channels[id].getInv_mode() == true && client[index].getInveted() == false)
            {
                sendToClient(JOIN_INVITEONLY_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
                continue;
            }
            if (channels[id].getEncrypted() == true)
            {
                if (args.size() < 2)
                {
                    sendToClient("NEED PASS\n", client[index].getSocket());
                    return;
                }
                if (args.size() >= 2  && args[1] != channels[id].getPass())
                {
                    sendToClient("WRONG PASS\n", client[index].getSocket());
                    return;
                }
            }
            if (channels[id].getLimit() != 0 && channels[id].getLimit() <= channels[id].getNumberOfClients())
            {
                sendToClient(JOIN_CHANNEL_ISFULL_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
                continue;
            }
            channels[id].addClient(client[index]);
            channels[id].sendToAll(JOIN_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), recipients[i]));
            sendToClient(JOIN_NAMREPLY_MSG(client[index].getNick(), recipients[i], channels[id].getClients()), client[index].getSocket());
            sendToClient(JOIN_ENDOFNAMES_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
        }
        else
        {
            Channel newChannel(recipients[i]);
            channels[channels.size()] = newChannel;
            channels[channels.size() - 1].addAdmin(client[index]);
            sendToClient(JOIN_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), recipients[i]), client[index].getSocket());
            sendToClient(JOIN_NAMREPLY_MSG(client[index].getNick(), recipients[i], channels[channels.size() - 1].getClients()), client[index].getSocket());
            sendToClient(JOIN_ENDOFNAMES_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
        }
    }
    
}

// INVITE Command ****************************************************************
void Command::INVITECommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(INVITE_NOTREGISTERED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 2)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "INVITE"), client[index].getSocket());
        return;
    }
    int id = checkNickUser(client, this->args[0], 1);
    int id2 = check_if_exist(this->args[1], channels);
    if (id == -1)
    {
        sendToClient(INVITE_NOSUCHNICK_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    if (id2 == -1)
    {
        sendToClient(INVITE_NOSUCHCHANNEL_MSG(client[index].getNick(), this->args[1]), client[index].getSocket());
        return;
    }
    if (channels[id2].checkNick(this->args[0]) != -1)
    {
        sendToClient(INVITE_USERONCHANNEL_MSG(client[index].getNick(), this->args[0], this->args[1]), client[index].getSocket());
        return;
    }
    if (channels[id2].checkNick(client[index].getNick()) == -1)
    {
        sendToClient(INVITE_NOTONCHANNEL_MSG(client[index].getNick(), this->args[1]), client[index].getSocket());
        return;
    }
    if (channels[id2].checkAdmin(client[index].getNick()) == -1)
    {
        sendToClient(INVITE_CHANOPRIVSNEEDED_MSG(client[index].getNick(), this->args[1]), client[index].getSocket());
        return;
    }
    if (channels[id2].checkNick(this->args[0]) != -1)
    {
        sendToClient(INVITE_USERONCHANNEL_MSG(client[index].getNick(), this->args[0], this->args[1]), client[index].getSocket());
        return;
    }
    client[id].setInveted(true);
    sendToClient(INVITE_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), this->args[0], this->args[1]), client[id].getSocket());
    sendToClient(INVITE_SUCCESS_MSG(client[index].getNick(), this->args[0], this->args[1]), client[index].getSocket());
}

// KICK Command ****************************************************************
void Command::KICKCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(KICK_NOTREGISTERED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 2)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "KICK"), client[index].getSocket());
        return;
    }
    if (this->args[0][0] != '#')
    {
        sendToClient(KICK_NOSUCHCHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    int id = check_if_exist(this->args[0], channels);
    if (id == -1)
    {
        sendToClient(KICK_NOSUCHCHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    if (channels[id].checkNick(client[index].getNick()) == -1)
    {
        sendToClient(KICK_NOTONCHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    if (channels[id].checkAdmin(client[index].getNick()) == -1)
    {
        sendToClient(KICK_CHANOPRIVSNEEDED_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    int id2 = checkNickUser(client, this->args[1], 1);
    if (id2 == -1)
    {
        sendToClient(KICK_NOSUCHNICK_MSG(client[index].getNick(), this->args[1]), client[index].getSocket());
        return;
    }
    if (channels[id].checkNick(this->args[1]) == -1)
    {
        sendToClient(KICK_USERNOTINCHANNEL_MSG(client[index].getNick(), this->args[1], this->args[0]), client[index].getSocket());
        return;
    }
    channels[id].sendToAll(KICK_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), this->args[1], this->args[0]));
    channels[id].removeClient(this->args[1]);
}

// PART Command ****************************************************************
void Command::PARTCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(PART_NOTREGISTERED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 1)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "PART"), client[index].getSocket());
        return;
    }
    std::stringstream ss(this->args[0]);
    std::string getline;
    std::vector<std::string> channelsName;
    while (std::getline(ss, getline, ','))
        channelsName.push_back(getline);
    for (size_t i = 0; i < channelsName.size(); i++)
    {
        if (channelsName[i].empty())
            continue;
        if (channelsName[i][0] != '#')
        {
            sendToClient(PART_NOSUCHCHANNEL_MSG(client[index].getNick(), channelsName[i]), client[index].getSocket());
            continue;
        }
        int id = check_if_exist(channelsName[i], channels);
        if (id == -1)
        {
            sendToClient(PART_NOTONCHANNEL_MSG(client[index].getNick(), channelsName[i]), client[index].getSocket());
            continue;
        }
        if (channels[id].checkNick(client[index].getNick()) == -1)
        {
            sendToClient(PART_NOTONCHANNEL_MSG(client[index].getNick(), channelsName[i]), client[index].getSocket());
            continue;
        }
        channels[id].sendToAll(PART_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), channelsName[i]));
        channels[id].removeClient(client[index].getNick());
    }
}

// MODE Command ****************************************************************
void Command::MODECommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(MODE_NOTREGISTERED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 2)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "MODE"), client[index].getSocket());
        return;
    }
    int id = check_if_exist(this->args[0], channels);
    if (id == -1)
    {
        sendToClient(MODE_NOSUCHCHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    if (channels[id].checkNick(client[index].getNick()) == -1)
    {
        sendToClient(MODE_NOTONCHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    if (channels[id].checkAdmin(client[index].getNick()) == -1)
    {
        sendToClient(MODE_CHANOPRIVSNEEDED_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    // this part need to be changed (ERROR MSG...)******************************************
    if (args[1] == "k" || args[1] == "-k" || args[1] == "+k")
    {
        mode_k(args, channels[id], client[index].getNick(), client[index].getSocket());
        return;
    }
    if (args[1] == "l" || args[1] == "-l" || args[1] == "+l")
    {
        mode_l(args, channels[id], client[index].getNick(), client[index].getSocket());
        return;
    }
    if (args[1] == "o" || args[1] == "-o" || args[1] == "+o")
    {
        mode_o(args, channels[id], client[index].getNick(), client[index].getSocket());
        return;
    }
    if (args[1] == "i" || args[1] == "-i" || args[1] == "+i")
    {
        mode_i(args, channels[id], client[index].getNick(), client[index].getSocket());
        return;
    }
    sendToClient("Command Done not implemented", client[index].getSocket());
}

void Command::execute(std::map<int, Client> &client, int index, std::map<int, Channel> &channel)
{
    if (!client[index].getIsRegistered())
        this->registerClient(client, index, channel);
    else
        this->commandHandler(client, index, channel);
}

void Command::commandHandler(std::map<int, Client> &client, int index, std::map<int, Channel> &channel)
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
            this->JOINCommand(client, index, channel);
            break;
        case PART:
            this->PARTCommand(client, index, channel);
            break;
        case MODE:
            this->MODECommand(client, index, channel);
            break;
        case TOPIC:
            //this->TOPICCommand(client, index, channel);
            break;
        case KICK:
            this->KICKCommand(client, index, channel);
            break;
        case INVITE:
            this->INVITECommand(client, index, channel);
            break;
        default:
            break;
    }
}