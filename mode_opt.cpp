#include "mode_opt.hpp"

int is_Number(const std::string &str)
{
    if (str.empty())
        return 0;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

void removeCharacter(std::string &str, char charToRemove)
{
    std::string result = "";
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] != charToRemove)
            result += str[i];
    }
    str = result;
}

void sendToclient(const std::string &msg, int clientSocket)
{
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

// option iiiiiiiiiiiiiiiiiiii ****************************************************************************
void mode_i(Channel &channel, Client client, bool &is_munis, std::string &msg)
{
    if (!is_munis)
    {
        if (channel.getInv_mode())
            return;
        channel.setInv_mode(true);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "+i"));
    }
    if (is_munis)
    {
        if (!channel.getInv_mode())
            return;
        channel.setInv_mode(false);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "-i"));
        removeCharacter(msg, 'i');
        return;
    }
}

// option ttttttttt***************************************************************************
void mode_tp(Channel &channel, Client client, bool &is_munis, std::string &msg)
{
    if (!is_munis)
    {
        if (channel.getTopicMode())
            return;
        channel.setTopicMode(true);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "+t"));
        return;
    }
    if (is_munis)
    {
        if (!channel.getTopicMode())
            return;
        channel.setTopicMode(false);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "-t " + "*"));
        removeCharacter(msg, 't');
        return;
    }
}

// option kkkkkkkkkkkk********************************************************************
void mode_k(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg)
{
    if (!is_munis)
    {
        if (1+count >= args.size())
            return;
       if (args[1 + count].empty())
            return;
        channel.setPass(args[1 + count]);
        channel.setEncrypted(true);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "+k " + args[1 + count]));
        count++;
        return;
    }
    if (is_munis)
    {
        if (channel.getPass().empty() || !channel.getEncrypted())
            return;
        channel.setPass("");
        channel.setEncrypted(false);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "-k " + "*"));
        removeCharacter(msg, 'k');
        return;
    }
}

// option oooooooooooooo************************************************************************
void mode_o(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg)
{
    if (!is_munis)
    {
        if (1+count >= args.size())
            return;
        if (args[1 + count].empty())
            return;
        if (channel.checkAdmin(args[1 + count]) != -1)
            count++;
        else if (channel.checkNick(args[1 + count]) != -1)
        {
            channel.addoperator(args[1 + count]);
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "+o " + args[1 + count]));
            count++;
        }
        else
        {
            sendToclient(MODE_NOTONCHANNEL_MSG(client.getNick(), channel.getName()), client.getSocket());
            count++;
        }
        return;
    }
    if (is_munis)
    {
        if (1+count >= args.size())
            return;
        if (args[1 + count].empty())
            return;
        if (channel.checkNick(args[1 + count]) == -1)
        {
            sendToclient(MODE_NOTONCHANNEL_MSG(client.getNick(), channel.getName()), client.getSocket());
            count++;
        }
        else if (channel.checkAdmin(args[1 + count]) != -1)
        {
            channel.removeoperator(args[1 + count]);
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "-o " + args[1 + count]));
            count++;
            removeCharacter(msg, 'o');
        }
        return;
    }
}

// option llllllllllllllllllllllllllll***********************************************************
void mode_l(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg)
{
    if (!is_munis)
    {
        if (1+count >= args.size())
            return;
        if (!is_Number(args[1 + count]))
        {
            std::cout << "not a number" << std::endl;
            count++;
            return;
        }
        channel.setLimit(atoi(args[1 + count].c_str()));
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "+l " + args[1 + count]));
        count++;
        return;
    }
    if (is_munis)
    {
        if (channel.getLimit() == 0)
            return;
        channel.setLimit(0);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), client.getIp(), channel.getName(), "-l " + "*"));
        removeCharacter(msg, 'l');
        return;
    }
}
