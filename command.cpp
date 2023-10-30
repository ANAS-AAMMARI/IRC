#include "command.hpp"
#include <ctime>
#include <cstdio>

std::vector<std::string> Command::listOfCommands;

void remove_duplicate(std::string &msg) {
    size_t i = 0;
    std::string temp = "";
    char prev_option = ' ';
    int check = 1;
    while (i < msg.size()) {
        if ((msg[i] == 'i' || msg[i] == 't') && check) {
            prev_option = msg[i++];
            check = 0;
        } else if ((msg[i] == 'i' || msg[i] == 't') && msg[i] == prev_option) {
            while (i < msg.size() && msg[i] == prev_option) {
                ++i;
            }
        } else {
            if (prev_option == 'i' || prev_option == 't') {
                temp += prev_option;
            }
            temp += msg[i++];
            prev_option = ' ';
            check = 1;
        }
    }
    if (prev_option == 'i' || prev_option == 't') {
        temp += prev_option;
    }
    msg = temp;
}


void    removeOperators(std::string &msg)
{
    size_t i = 0;
    std::string temp = "";
    char prev_operator;
    while (i < msg.size())
    {
        if (msg[i] == '+' || msg[i] == '-')
        {
            prev_operator = msg[i];
            i++;
        }
        else
        {
            if (prev_operator == '+' || prev_operator == '-')
                temp += prev_operator;
            temp += msg[i++];
            prev_operator = ' ';
        }
    }
    msg = temp;
}

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
    Command::listOfCommands.push_back("PONG");
    Command::listOfCommands.push_back("BIMO");
    Command::listOfCommands.push_back("QUIT");  
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
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        return "NULL";
    }

    struct hostent *host = gethostbyname(hostname);
    if (host == NULL)
    {
        return "NULL";
    }

    char *ipAddress = inet_ntoa(*(struct in_addr *)host->h_addr);
    if (ipAddress == NULL)
    {
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
        index = msg.size();
    }
    std::string result;
    bool previousIsSpace = false;
    for (size_t i = 0; i < temp.size(); i++)
    {
        if (std::isspace(temp[i]))
        {
            if (!previousIsSpace)
            {
                result += ' ';
            }
            previousIsSpace = true;
        }
        else
        {
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
    trimString(this->msg);
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
    if (this->indexOfCommand == PRIVMSG)
    {  
        std::string temp2 = ss.str().substr(7);
        trimString(temp2);
        size_t index = temp2.find(' ');
        if (index != std::string::npos)
        {
            this->args.push_back(temp2.substr(0, index));
            temp2 = temp2.substr(index + 1);
            trimString(temp2);
            this->args.push_back(temp2);
        }
        else
            this->args.push_back(temp2);
        return;
    }
    this->msg = ss.str().substr(this->command.size());
    removeSpaces(this->msg);
    std::stringstream ss1(this->msg);

    size_t index = this->msg.find(':');
    if (index != std::string::npos)
    {
        std::getline(ss1, temp, ':');
        std::stringstream ss2(temp);
        while (std::getline(ss2, temp, ' '))
            this->args.push_back(temp);
        std::getline(ss1, temp);
        this->args.push_back(temp);
    }
    else
        while (std::getline(ss1, temp, ' '))
            this->args.push_back(temp);
}

std::string Command::getCurrentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];

    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %b %d %Y at %H:%M:%S %p", &tstruct);

    return std::string(buf);
}

time_t getCreationTime(const std::string creation_time) {
    const char* dateString = creation_time.c_str();
    struct tm timeinfo;

    const char* format = "%a %b %d %Y at %H:%M:%S";

    if (strptime(dateString, format, &timeinfo) != NULL) {
        timeinfo.tm_isdst = -1;
        return mktime(&timeinfo);
    }
    return 0;
}

bool Command::checkNick(const std::string &nickname)
{
    if (nickname.empty() || nickname[0] == ':' || nickname.find(' ') != std::string::npos)
        return false;
    const std::string validChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]{}\\`^|_";

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

std::string Command::getHostName()
{
    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) != 0)
    {
        return "NULL";
    }

    struct hostent *host = gethostbyname(hostname);
    if (host == NULL)
    {
        return "NULL";
    }

    return std::string(host->h_name);
}


void Command::registerClient(std::map<int, Client> &client, int index, std::map<int, Channel> &channel, Server &server)
{
    this->commandHandler(client, index, channel, server);
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
        std::string msg = "";
        if (getHostName() != "NULL")
            msg = ":" + getHostName() + " 464 NOTICE AUTH :*** Invalid password\r\n";
        sendToClient(msg, index);
        return;
    }
    client[index].setIsValidPass(true);
}

// NICK Command ***********************************************************
void Command::NICKCommand(std::map<int, Client> &client, int index)
{
    if (client[index].getIsValidPass() == false)
    {
        sendToClient(PASS_REQ_MSG(getHostName()), client[index].getSocket());
        return;
    }
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
    if (client[index].getIsValidPass() == false)
    {

        sendToClient(PASS_REQ_MSG(getHostName()), client[index].getSocket());
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
                channel[id].sendToAllButOne(PRIVMSG_AWAY_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), this->args[0], this->args[1]), client[index].getNick());
            }
            else
                sendToClient(PRIVMSG_NOSUCHNICK_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
        }
        else
        {
            int id = checkNickUser(client, recipients[i], 1);
            if (id != -1)
                sendToClient(PRIVMSG_AWAY_MSG(client[index].getNick(),
                                              client[index].getUser(), getLoclalIp(), recipients[i], this->args[1]),
                             client[id].getSocket());
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
        if (recipients[i].size() < 2)
        {
            sendToClient(REQUIRED_MSG(client[index].getNick(), "JOIN"), client[index].getSocket());
            continue;
        }
        if (recipients[i][0] != '#')
        {
            sendToClient(JOIN_NO_SUCH_CHANNEL_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
            continue;
        }
        int id = check_if_exist(recipients[i], channels);
        if (id != -1)
        {
            // this part need error msg *********************************************************
            if (channels[id].checkNick(client[index].getNick()) != -1)
                continue;
            if (channels[id].getEncrypted() == true)
            {
                if (args.size() < 2 || (args.size() >= 2 && args[1] != channels[id].getPass()))
                {
                    sendToClient(JOIN_BAD_CHANNEL_KEY_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
                    return;
                }
            }
            if (channels[id].getInv_mode() == true && channels[id].checkInvited(client[index].getNick()) == -1)
            {
                sendToClient(JOIN_INVITE_ONLY_CHAN_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
                continue;
            }
            if (channels[id].getLimit() != 0 && channels[id].getLimit() <= channels[id].getNumberOfClients())
            {
                sendToClient(JOIN_CHANNEL_IS_FULL_MSG(client[index].getNick(), recipients[i]), client[index].getSocket());
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
            newChannel.setCreation_time(getCurrentDateTime());
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
    channels[id2].addInvited(client[id].getNick());
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
    if (channels[id].getNumberOfClients() == 0)
        channels.erase(id);
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
        if (channels[id].getNumberOfClients() == 0)
            channels.erase(id);
    }
}

// TOPIC Command ****************************************************************
void Command::TOPICCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(TOPIC_NOTREGISTERED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 1)
    {
        sendToClient(REQUIRED_MSG(client[index].getNick(), "TOPIC"), client[index].getSocket());
        return;
    }
    int id = check_if_exist(this->args[0], channels);
    if (id == -1)
    {
        sendToClient(TOPIC_NOSUCHCHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    if (channels[id].checkNick(client[index].getNick()) == -1)
    {
        sendToClient(TOPIC_NOTONCHANNEL_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    if (channels[id].checkAdmin(client[index].getNick()) != -1 || channels[id].getTopicMode() == false)
    {
        if (this->args.size() < 2)
        {
            if (channels[id].getTopic().empty())
                sendToClient(TOPIC_NOTOPIC_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
            else
                sendToClient(TOPIC_MSG(client[index].getNick(), this->args[0], channels[id].getTopic()), client[index].getSocket());
            return;
        }
        if (this->args[1].empty())
        {
            channels[id].setTopic(":");
            channels[id].sendToAll(TOPIC_SET_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), this->args[0], channels[id].getTopic()));
            return;
        }
        std::string msg = "";
        for (size_t i = 1; i < this->args.size(); i++)
            msg += this->args[i] + " ";
        channels[id].setTopic(msg);
        channels[id].sendToAll(TOPIC_SET_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp(), this->args[0], channels[id].getTopic()));
    }
    else
    {
        if (this->args.size() < 2)
        {
            if (channels[id].getTopic().empty())
                sendToClient(TOPIC_NOTOPIC_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
            else
            {
                sendToClient(TOPIC_MSG(client[index].getNick(), this->args[0], channels[id].getTopic()), client[index].getSocket());
                std::string msg = ":" + client[index].getNick() + "set the topic at " + channels[id].getTopic_time();
                sendToClient(msg, client[index].getSocket());
            }    
            return;
        }
        sendToClient(TOPIC_CHANOPRIVSNEEDED_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
}

// QUIT Command ****************************************************************
void Command::QUITCommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels, Server &server)
{
    sendToClient(QUIT_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp()), client[index].getSocket());
    for (std::map<int, Channel>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->second.checkNick(client[index].getNick()) != -1)
        {
            it->second.removeClient(client[index].getNick());
            it->second.sendToAll(QUIT_MSG(client[index].getNick(), client[index].getUser(), getLoclalIp()));
        }
    }
    sendToClient(QUIT_ERROR_MSG(this->getLoclalIp()), client[index].getSocket());
    std::cout << "Client with socket number " << client[index].getSocket() << " disconnected" << std::endl;
    close(client[index].getSocket());
    client.erase(client[index].getSocket());
    std::vector<struct pollfd> tmp = server.getPollfds();
    for (size_t i = 0; i < tmp.size(); i++)
    {
        if (tmp[i].fd == client[index].getSocket())
        {
            tmp.erase(tmp.begin() + i);
            break;
        }
    }
}

std::string toString(int num)
{
    std::string result;
    std::stringstream ss;
    ss << num;
    ss >> result;
    return result;
}

// MODE Command ****************************************************************
void Command::MODECommand(std::map<int, Client> &client, int index, std::map<int, Channel> &channels)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(MODE_NOTREGISTERED_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() < 1)
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
    std::string msg = "";
    if (this->args.size() == 1 || (args.size() == 2 && args[1] == "+sn"))
    {
        bool plus = false;
        if (channels[id].getInv_mode())
        {
            plus = true;
            msg+="i";
        } 
        if (channels[id].getEncrypted())
        {
            plus = true;
            msg+="k";
        }    
        if (channels[id].getLimit() != 0)
        {
            plus = true;
            msg+="l";
        }    
        if (channels[id].getTopicMode())
        {
            plus = true;
            msg+="t";
        }
        if (plus)
            msg = "+" + msg;
        if (channels[id].getEncrypted())
            msg += " " + channels[id].getPass();
        if (channels[id].getLimit() != 0)
            msg += " " + toString(channels[id].getLimit());
        if (!msg.empty())
            sendToClient(MODE_SUCCESS_MSG(client[index].getNick(), this->args[0], msg), client[index].getSocket());
        time_t timestamp = getCreationTime(channels[id].getCreation_time());
        std::string str = toString(timestamp);
        sendToClient(JOIN_CREATE_TIME_MSG(client[index].getNick(), this->args[0], str), client[index].getSocket());
        return;
    }
    if (channels[id].checkAdmin(client[index].getNick()) == -1)
    {
        sendToClient(MODE_CHANOPRIVSNEEDED_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
        return;
    }
    bool is_minus = false;
    size_t count = 1;
    int check = 0;
    remove_duplicate(this->args[1]);
    removeOperators(this->args[1]);
    int size = this->args[1].size();
    for (int j = 0; j < size; j++)
    {
        if (check == 3 || check == 4)
            return;
        if (size > 1)
        {
            check = 1;
            if (j + 1 == size)
                check = 2;
        }
        if (args[1][j] == '+' || args[1][j] == 'i')
        {
            if (args[1][j] == 'i')
                mode_i(args,channels[id], client[index], is_minus, msg, check);
            else
                is_minus = false;
            continue;
        }
        if (args[1][j] == '-')
        {
            is_minus = true;
            continue;
        }
        if (args[1][j] == 'l')
        {
            mode_l(args, channels[id], client[index], is_minus, count, msg, check, j);
            continue;
        }
        if (args[1][j] == 'k')
        {
            mode_k(args, channels[id], client[index], is_minus, count, msg, check, j);
            continue;
        }
        if (args[1][j] == 'o')
        {
            mode_o(args, channels[id], client[index], is_minus, count, msg, check, j);
            continue;
        }
        if (args[1][j] == 't')
        {
            mode_tp(args, channels[id], client[index], is_minus, msg, check);
            continue;
        }
        else
        {
            sendToClient(MODE_BADCHANMODE_MSG(client[index].getNick(), this->args[0]), client[index].getSocket());
            return;
        }
    }
}

void Command::execute(std::map<int, Client> &client, int index, std::map<int, Channel> &channel, Server &server)
{
    if (!client[index].getIsRegistered())
        this->registerClient(client, index, channel, server);
    else
        this->commandHandler(client, index, channel, server);   
}

void Command::commandHandler(std::map<int, Client> &client, int index, std::map<int, Channel> &channel, Server &server)
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
        this->TOPICCommand(client, index, channel);
        break;
    case KICK:
        this->KICKCommand(client, index, channel);
        break;
    case INVITE:
        this->INVITECommand(client, index, channel);
        break;
    case PONG:
        break;
    case BIMO:
        this->BIMOCommand(client, index);
        break;
    case QUIT:
        this->QUITCommand(client, index, channel, server);
        break;
    default:
        break;
    }
}

void    Command::BIMOCommand(std::map<int, Client> &client, int index)
{
    if (!client[index].getIsRegistered())
    {
        sendToClient(ALREADY_MSG(client[index].getNick()), client[index].getSocket());
        return;
    }
    if (this->args.size() == 0)
    {
        std::string msg = "Hi, i'm bimo, i can do some stuff for you, for example :";
        sendToClient(MSG(client[index].getNick(), msg), client[index].getSocket());
        sendToClient(MSG(client[index].getNick(), "[1] - say Hi"), client[index].getSocket());
        sendToClient(MSG(client[index].getNick(), "[2] - mini_game"), client[index].getSocket());
        sendToClient(MSG(client[index].getNick(), "[3] - tell you best anime"), client[index].getSocket());
        sendToClient(MSG(client[index].getNick(), "[4] - tell you a joke"), client[index].getSocket());
        sendToClient(MSG(client[index].getNick(), "[5] - say bye"), client[index].getSocket());
        msg = "please enter the number of the command you want to execute (in this format, bot_name [number]):";
        sendToClient(MSG(client[index].getNick(), msg), client[index].getSocket());
        return;
    }
    if (this->args.size() > 1)
    {
        std::string msg = "please enter the number of the command you want to execute (in this format, bimo [number]) : ";
        sendToClient(MSG(client[index].getNick(), msg), client[index].getSocket());
    }    
    else if (this->args.size() == 1)
    {
        if (client[index].getGameStart()){
            if (isNumber(this->args[0]))
            {
                client[index].setguess_number(atoi(this->args[0].c_str())); 
                mini_game(client, index, 0);
            }
            else
            {
                std::string msg = "please enter number between 1~100, it is simple 👏, for example : bimo 23";
                sendToClient(MSG(client[index].getNick(), msg), client[index].getSocket());
            }
        }
        else if (args[0] == "[1]")
                sendToClient(MSG(client[index].getNick(), ":👋Hii '_', what do you expect me to say -_- lol :)"), client[index].getSocket());
        else if (args[0] == "[2]")
        {
            client[index].setGameStart(1);
            mini_game(client, index, 1);
        }
        else if (args[0] == "[3]")
            sendToClient(MSG(client[index].getNick(), "Best Anime: shingeki no kyojin 🤩"), client[index].getSocket());
        else if (args[0] == "[4]")
            sendToClient(MSG(client[index].getNick(), "Joke: Why do programmers prefer darkmode, Because light attracts bugs 😂"), client[index].getSocket());
        else if (args[0] == "[5]")
            sendToClient(MSG(client[index].getNick(), "Bye: see you soon 🙏"), client[index].getSocket());
        else
            sendToClient(MSG(client[index].getNick(), "please enter a valid command number, for example : bimo [1]"), client[index].getSocket());
        return;
    }
    return;
}

void    Command::mini_game(std::map<int, Client> &client, int index, int x)
{
    if (x)
    {
        std::string msg = "Welcome to this mini_game, the main idea is to guess a Number between 1~100, and see how many times you trying before get it 🔥";
        srand(time(NULL));
        client[index].setRandomNumber(rand() % 100 + 1);
        client[index].setTryed(0);
        client[index].setguess_number(0);
        sendToClient(MSG(client[index].getNick(), msg), client[index].getSocket());
    }
    else
    {
        if (!client[index].getguess_number())
            sendToClient(MSG(client[index].getNick(), "why entre 0? is not between 0~100, i think im already say that,Entre just Numbers between 1~100 🤔:"), client[index].getSocket());
        else if (client[index].getguess_number() == client[index].getRandomNumber())
        {
            client[index].increaseTryed();
            std::stringstream ss;
            std::stringstream ss2;
            ss << client[index].getTryed();
            ss2 <<client[index].getguess_number(); 
            std::string result = ss.str();
            std::string result2 = ss2.str();
            std::string msg = "✅you guessed it in " + result + " tryes, the number was "+ result2+ "🎉";
            sendToClient(MSG(client[index].getNick(), msg), client[index].getSocket());
            msg = "i hope you enjoyed the game 🙏, see you soon :)";
            sendToClient(MSG(client[index].getNick(), msg), client[index].getSocket());
            client[index].setGameStart(0);
            client[index].setguess_number(-1);
            client[index].setTryed(0);
        }
        else if (client[index].getguess_number() > 100)
        {
            client[index].increaseTryed();
            sendToClient(MSG(client[index].getNick(), "TOO HIGH❌"), client[index].getSocket());
            sendToClient(MSG(client[index].getNick(), "please enter a number between 1~100 : "), client[index].getSocket());
        }
        else if (client[index].getguess_number() > client[index].getRandomNumber())
        {
            client[index].increaseTryed();
            sendToClient(MSG(client[index].getNick(), "Too high 📈, try again :"), client[index].getSocket());
        }
        else
        {
            client[index].increaseTryed();
            sendToClient(MSG(client[index].getNick(), "Too low 📉, try again :"), client[index].getSocket());
        }
    }
}