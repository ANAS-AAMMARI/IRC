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

static std::string getLocalIP()
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

void removeCharacter(std::string &str, char charToRemove)
{
    std::string result = "";
    for (size_t i = 0; i < str.size(); ++i)
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
void mode_i(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, std::string &msg, int &check)
{
    if (!is_munis)
    {
        if (channel.getInv_mode() && check != 2)
            return;
        channel.setInv_mode(true);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+i"));
        if (check == 2)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
                temp = args[1];
            else
                temp = "+" + args[1];
            while (i < args.size())
                temp += " " + args[i++];
            if (args.size() == 2)
                temp += " *";
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        return;
    }
    if (is_munis)
    {
        if (!channel.getInv_mode() && check != 2)
            return;
        channel.setInv_mode(false);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-i"));
        if (check == 2)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
                temp = args[1];
            else
                temp = "+" + args[1];
            while (i < args.size())
                temp += " " + args[i++];
            if (args.size() == 2)
                temp += " *";
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        removeCharacter(msg, 'i');
        return;
    }
}

// option ttttttttt***************************************************************************
void mode_tp(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, std::string &msg, int &check)
{
    if (!is_munis)
    {
        if (channel.getTopicMode() && check != 2)
            return;
        channel.setTopicMode(true);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+t"));
        if (check == 2)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
                temp = args[1];
            else
                while (i < args.size())
                    temp += " " + args[i++];
            if (args.size() == 2)
                temp += " *";
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        return;
    }
    if (is_munis)
    {
        if (!channel.getTopicMode() && check != 2)
            return;
        channel.setTopicMode(false);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-t " + "*"));
        if (check == 2)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
                temp = args[1];
            else
                temp = "+" + args[1];
            while (i < args.size())
                temp += " " + args[i++];
            if (args.size() == 2)
                temp += " *";
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        removeCharacter(msg, 't');
        return;
    }
}

// option kkkkkkkkkkkk********************************************************************
void mode_k(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg, int &check, int index)
{
    if (!is_munis)
    {
       if (args[1 + count].empty() && !check)
            return;
        if (args[1 + count].empty() && (check || check == 2))
            check = 3;
        else
        {
            channel.setPass(args[1 + count]);
            channel.setEncrypted(true);
        }
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+k " + args[1 + count]));
        if (check == 2 || check == 3)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
            {
                if (check == 3)
                    temp = args[1].substr(0, index);
                else
                    temp = args[1];
            }
            else
            {
                if (check == 3)
                    temp = "+" + args[1].substr(0, index);
                else
                    temp = "+" + args[1];
            }
            while (i < args.size())
                temp += " " + args[i++];
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        count++;
        return;
    }
    if (is_munis)
    {
        channel.setPass("");
        channel.setEncrypted(false);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-k " + "*"));
        if (check == 2)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
                temp = args[1];
            else
                temp = "+" + args[1];
            while (i < args.size())
                temp += " " + args[i++];
            if (args.size() == 2)
                temp += " *";
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        count++;
        removeCharacter(msg, 'k');
        return;
    }
}

// option oooooooooooooo************************************************************************
void mode_o(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg, int &check, int index)
{
    if (!is_munis)
    {
        if (args[1 + count].empty() && !check)
            return;
        if (args[1 + count].empty() && (check || check == 2))
            check = 3;
        if (channel.checkNick(args[1 + count]) != -1)
        {
            channel.addoperator(args[1 + count]);
            if (!check)
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+o " + args[1 + count]));
            if (check == 2 || check == 3)
            {
                size_t i = 2;
                std::string temp = "";
                if (args[1][0] == '-' || args[1][0] == '+')
                {
                    if (check == 3)
                        temp = args[1].substr(0, index);
                    else
                        temp = args[1];
                }
                else
                {
                    if (check == 3)
                        temp = "+" + args[1].substr(0, index);
                    else
                        temp = "+" + args[1];
                }
                while (i < args.size())
                    temp += " " + args[i++];
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
            }
            count++;
            return;
        }
        sendToclient(MODE_USERNOTINCHANNEL_MSG(client.getNick(), args[1 + count], channel.getName()), client.getSocket());
        count++;
        return;
    }
    if (is_munis)
    {
        if (args[1 + count].empty() && !check)
            return;
        if (args[1 + count].empty() && (check || check == 2))
            check = 3;
        if (channel.checkAdmin(args[1 + count]) != -1 || check == 3)
        {
            if (check != 3)
                channel.removeoperator(args[1 + count]);
            if (!check)
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-o " + args[1 + count]));
            if (check == 2 || check == 3)
            {
                size_t i = 2;
                std::string temp = "";
                if (args[1][0] == '-' || args[1][0] == '+')
                {
                    if (check == 3)
                        temp = args[1].substr(0, index);
                    else
                        temp = args[1];
                }
                else
                {
                    if (check == 3)
                        temp = "+" + args[1].substr(0, index);
                    else
                        temp = "+" + args[1];
                }
                while (i < args.size())
                    temp += " " + args[i++];
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
            }
                count++;
                if (check != 3)
                    removeCharacter(msg, 'o');
                return;
        }
        if (channel.checkNick(args[1 + count]) != -1)
        {
            sendToclient(MODE_CHANOPRIVSNEEDED_MSG(client.getNick(), channel.getName()), client.getSocket());
            count++;
            return;
        }
        sendToclient(MODE_USERNOTINCHANNEL_MSG(client.getNick(), args[1 + count], channel.getName()), client.getSocket());
        count++;
        return;
    }
}

// option llllllllllllllllllllllllllll***********************************************************
void mode_l(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg, int &check, int index)
{
    if (!is_munis)
    {
        if (!is_Number(args[1 + count]) && !check)
            return;
        if (!is_Number(args[1 + count]) && (check || check == 2))
            check = 3;
        else
            channel.setLimit(atoi(args[1 + count].c_str()));
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+l " + args[1 + count]));
        if (check == 2 || check == 3)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
            {
                if (check == 3)
                    temp = args[1].substr(0, index);
                else
                    temp = args[1];
            }
            else
            {
                if (check == 3)
                    temp = "+" + args[1].substr(0, index);
                else
                    temp = "+" + args[1];
            }
            while (i < args.size())
                temp += " " + args[i++];
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        count++;
        return;
    }
    if (is_munis)
    {
        channel.setLimit(0);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-l"));
        if (check == 2)
        {
            size_t i = 2;
            std::string temp = "";
            if (args[1][0] == '-' || args[1][0] == '+')
                temp = args[1];
            else
                temp = "+" + args[1];
            while (i < args.size())
                temp += " " + args[i++];
            if (args.size() == 2)
                temp += " *";
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), temp));
        }
        count++;
        removeCharacter(msg, 'l');
        return;
    }
}
