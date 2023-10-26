#include "mode_opt.hpp"

static std::string getLocalIP()
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

void removeCharacter(std::string& str, char charToRemove) {
    std::string result = "";
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] != charToRemove) {
            result += str[i];
        }
    }
    str = result;
}

void sendToclient(const std::string &msg, int clientSocket)
{
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

// option iiiiiiiiiiiiiiiiiiii ****************************************************************************
void    mode_i(std::vector<std::string> &args ,Channel &channel, Client client, bool &is_munis, std::string &msg, int check)
{
    if (!is_munis)
    {
        channel.setInv_mode(true);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+i"));
        if (check == 2)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+" + args[1]));
        return;
    }
    if (is_munis) 
    {   
        channel.setInv_mode(false);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-i"));
        else
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), args[1]));
        removeCharacter(msg, 'i');
        return;
    }
}

// option ttttttttt***************************************************************************
void    mode_tp(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, std::string &msg, int check)
{
    if (!is_munis)
    {
       channel.setTopicMode(true);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+t"));
        if (check == 2)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+" + args[1]));
        return; 
    }
    if (is_munis)
    {
        channel.setTopicMode(false);
        if (!check)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-t"));
        if (check == 2)
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), args[1]));
        removeCharacter(msg, 't');
        return;
    }
}


// option kkkkkkkkkkkk********************************************************************
void    mode_k(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg, int check)
{
    if (args.size() >= 1 + count)
    {
        if (!is_munis)
        {
            if (args[1 + count].empty())
                return;
            channel.setPass(args[1 + count]);
            channel.setEncrypted(true);
            if (!check)
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+k"));
            if (check == 2)
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+" + args[1]));
            count++;
            return;
        }
        if (is_munis) 
        {   
            if (args[1 + count] == ":")
            {
                channel.setPass("");
                channel.setEncrypted(false);
                if (!check)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-k"));
                if (check == 2)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), args[1]));
                count++;
                removeCharacter(msg, 'k');
                return;
            }
        }
    }
    sendToclient(REQUIRED_MSG(client.getNick(), "MODE"), client.getSocket());
}



// option oooooooooooooo************************************************************************
void    mode_o(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg, int check)
{
    if (args.size() >= 1 + count)
    {
        if (!is_munis)
        {
            if (args[1 + count].empty())
                return;
            if (channel.checkAdmin(args[1 + count]) != -1)
            {
                count++;
                return;
            }
            if (channel.checkNick(args[1 + count]) != -1)
            {
                channel.addoperator(args[1 + count]);
                if (!check)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+o"));
                if (check == 2)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+" + args[1]));
                count++;
                return;
            }
            sendToclient(MODE_NOTONCHANNEL_MSG(client.getNick(), channel.getName()), client.getSocket());
            count++;
            return;
        }
        if (is_munis) 
        {
            if (channel.checkAdmin(args[1 + count]) != -1)
            {
                channel.removeoperator(args[1 + count]);
                if (!check)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-o"));
                if (check == 2)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), args[1]));
                count++;
                removeCharacter(msg, 'o');
                return;
            }
            if (channel.checkNick(args[1 + count]) != -1)
            {
                sendToclient(MODE_CHANOPRIVSNEEDED_MSG(client.getNick(), channel.getName()), client.getSocket());
                count++;
                return;
            }
            sendToclient(MODE_NOTONCHANNEL_MSG(client.getNick(), channel.getName()), client.getSocket());
            count++;
            return;
        }
        sendToclient(REQUIRED_MSG(client.getNick(), "MODE"), client.getSocket());
    }
}

// option llllllllllllllllllllllllllll***********************************************************
void    mode_l(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg, int check)
{
    if (args.size() >= 1 + count)
    {
        if (!is_munis)
        {
            if (args[1 + count].empty())
                return;
            channel.setLimit(atoi(args[1 + count].c_str()));
            if (!check)
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+l"));
            if (check == 2)
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+" + args[1]));
            count++;
            return;
        }
        if (is_munis)
        {   
            if (args[1 + count] == ":")
            {
                channel.setLimit(0);
                if (!check)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-l"));
                if (check == 2)
                    channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), args[1]));
                count++;
                removeCharacter(msg, 'l');
                return;
            }
        }
    }
    sendToclient(REQUIRED_MSG(client.getNick(), "MODE"), client.getSocket());
}
