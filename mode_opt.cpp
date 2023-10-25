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

void sendToclient(const std::string &msg, int clientSocket)
{
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

// option of MODE COMMAND

// option iiiiiiiiiiiiiiiiiiii ****************************************************************************
void    mode_i(Channel &channel, Client client, bool &is_munis)
{
    if (is_munis == false)
    {
        channel.setInv_mode(true);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+i"));
        return;
    }
    if (is_munis == true) 
    {   
        channel.setInv_mode(false);
        channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-i"));
        return;
    }
}

// option ttttttttt***************************************************************************
/*void    mode_t(std::vector<std::string> &args, Channel &channel, std::string nick, int socket)
{
    nick = "";
    if (args[1] == "t" || args[1] == "+t")
        {
            channel.setTopic_mode(true);
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel is topic only mode\n");
            return;
        }
    if (args[1] == "-t") 
        {   
            if (args[2].empty())
            {
                channel.setTopic_mode(false);
                channel.sendToAll("MODE : " + args[1] + " Done, Now channel is not topic only mode\n");
                return;
            }
        }
}*/


// option kkkkkkkkkkkk********************************************************************
void    mode_k(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count)
{
    if (args.size() >= 1 + count)
    {
        if (is_munis == false)
        {
            if (args[1 + count].empty())
                return;
            channel.setPass(args[1 + count]);
            channel.setEncrypted(true);
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+k"));
            count++;
            return;
        }
        if (is_munis == true) 
        {   
            if (args[1 + count] == ":")
            {
                channel.setPass("");
                channel.setEncrypted(false);
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-k"));
                count++;
                return;
            }
        }
    }
    sendToclient(REQUIRED_MSG(client.getNick(), "MODE"), client.getSocket());
}



// option oooooooooooooo************************************************************************
void    mode_o(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count)
{
    if (args.size() >= 1 + count)
    {
        if (is_munis == false)
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
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+o"));
                count++;
                return;
            }
            sendToclient(MODE_NOTONCHANNEL_MSG(client.getNick(), channel.getName()), client.getSocket());
            count++;
            return;
        }
        if (is_munis == true) 
        {
            if (channel.checkAdmin(args[1 + count]) != -1)
            {
                channel.removeoperator(args[1 + count]);
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-o"));
                count++;
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
void    mode_l(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count)
{
    if (args.size() >= 1 + count)
    {
        if (is_munis == false)
        {
            if (args[1 + count].empty())
                return;
            channel.setLimit(atoi(args[1 + count].c_str()));
            channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "+l"));
            count++;
            return;
        }
        if (is_munis == true)
        {   
            if (args[1 + count] == ":")
            {
                channel.setLimit(0);
                channel.sendToAll(MODE_MSG(client.getNick(), client.getUser(), getLocalIP(), channel.getName(), "-l"));
                count++;
                return;
            }
        }
    }
    sendToclient(REQUIRED_MSG(client.getNick(), "MODE"), client.getSocket());
}

/*  if (args[1] == "k" || args[1] == "-k" || args[1] == "+k")
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
    sendToClient("Command Done not implemented", client[index].getSocket());*/