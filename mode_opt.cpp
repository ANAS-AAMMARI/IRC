#include "mode_opt.hpp"

void sendToclient(const std::string &msg, int clientSocket)
{
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

// option of MODE COMMAND

// option iiiiiiiiiiiiiiiiiiii ****************************************************************************
void    mode_i(std::vector<std::string> &args, Channel &channel, std::string nick, int socket, bool &is_munis)
{
    nick = "";
    socket = 0;
    if (is_munis == false)
        {
            channel.setInv_mode(true);
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel invite only mode\n");
            return;
        }
    if (is_munis == true) 
        {   
            
            channel.setInv_mode(false);
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel is not invite only mode\n");
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
void    mode_k(std::vector<std::string> &args, Channel &channel, std::string nick, int socket, bool &is_munis, size_t &count)
{
    nick = "";
    if (args.size() >= 1 + count)
    {
        if (is_munis == false)
        {
            if (args[1 + count].empty())
                return;
            channel.setPass(args[1 + count]);
            channel.setEncrypted(true);
            channel.sendToAll("MODE : " + args[1 + count] + " Done, Now channel is encrypted\n");
            count++;
            return;
        }
        if (is_munis == true) 
        {   
            if (args[1 + count] == ":")
            {
                channel.setPass("");
                channel.setEncrypted(false);
                channel.sendToAll("MODE : " + args[1 + count] + " Done, Now channel is not encrypted\n");
                count++;
                return;
            }
        }
    }
    sendToclient(" need more arguments(pass)\n", socket);
}



// option oooooooooooooo************************************************************************
void    mode_o(std::vector<std::string> &args, Channel &channel, std::string nick, int socket, bool &is_munis, size_t &count)
{
    nick = "";
    if (args.size() >= 1 + count)
    {
        if (is_munis == false)
        {
            if (args[1 + count].empty())
                return;
            if (channel.checkAdmin(args[1 + count]) != -1)
            {
                sendToclient("MODE : " + args[1 + count] + " is already an operator\n", socket);
                count++;
                return;
            }
            if (channel.checkNick(args[1 + count]) != -1)
            {
                channel.addoperator(args[1 + count]);
                channel.sendToAll("MODE : " + args[1 + count] + " Done, Now channel have one more operator\n");
                count++;
                return;
            }
            sendToclient("MODE : " + args[1 + count] + " is not on channel\n", socket);
            count++;
            return;
        }
        if (is_munis == true) 
        {
            if (channel.checkAdmin(args[1 + count]) != -1)
            {
                channel.removeoperator(args[1 + count]);
                channel.sendToAll("MODE : " + args[1 + count] + " Done, Now channel have -1 operator\n");
                count++;
                return;
            }
            if (channel.checkNick(args[1 + count]) != -1)
            {
                sendToclient("MODE : " + args[1 + count] + " is not an operator\n", socket);
                count++;
                return;
            }
            sendToclient("MODE : " + args[1 + count] + " is not on channel\n", socket);
            count++;
            return;
        }
        sendToclient(" need more arguments(operator)\n", socket);
    }
}

// option llllllllllllllllllllllllllll***********************************************************
void    mode_l(std::vector<std::string> &args, Channel &channel, std::string nick, int socket, bool &is_munis, size_t &count)
{
    nick = "";
    if (args.size() >= 1 + count)
    {
        if (is_munis == false)
        {
            if (args[1 + count].empty())
                return;
            channel.setLimit(atoi(args[1 + count].c_str()));
            channel.sendToAll("MODE : " + args[1 + count] + " Done, Now channel is limited\n");
            count++;
            return;
        }
        if (is_munis == true) 
        {   
            if (args[1 + count] == ":")
            {
                channel.setLimit(0);
                channel.sendToAll("MODE : " + args[1 + count] + " Done, Now channel is not limited\n");
                count++;
                return;
            }
        }
    }
    sendToclient(" need more arguments(limit)\n", socket);
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