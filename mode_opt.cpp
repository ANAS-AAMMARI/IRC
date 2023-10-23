#include "mode_opt.hpp"

void sendToclient(const std::string &msg, int clientSocket)
{
    send(clientSocket, msg.c_str(), msg.size(), 0);
}

void    mode_k(std::vector<std::string> &args, Channel &channel, std::string nick, int socket)
{
    nick = "";
    if (args.size() < 3)
        {   
            sendToclient(" need more arguments(password)\n", socket);
            return;
        }
        if (args[1] == "k" || args[1] == "+k")
        {
            channel.setPass(args[2]);
            channel.setEncrypted(true);
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel is encrypted\n");
            return;
        }
        if (args[1] == "-k") 
        {   
            if (args[2].empty())
            {
                channel.setPass("");
                channel.setEncrypted(false);
                channel.sendToAll("MODE : " + args[1] + " Done, Now channel is not encrypted\n");
                return;
            }
        }
}

void    mode_l(std::vector<std::string> &args, Channel &channel, std::string nick, int socket)
{
    nick = "";
    if (args.size() < 3)
        {   
            sendToclient(" need more arguments(limit)\n", socket);
            return;
        }
        if (args[1] == "l" || args[1] == "+l")
        {
            channel.setLimit(atoi(args[2].c_str()));
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel is limited\n");
            return;
        }
        if (args[1] == "-l") 
        {   
            if (args[2].empty())
            {
                channel.setLimit(0);
                channel.sendToAll("MODE : " + args[1] + " Done, Now channel is not limited\n");
                return;
            }
        }
}

void    mode_o(std::vector<std::string> &args, Channel &channel, std::string nick, int socket)
{
    nick = "";
    if (args.size() < 3)
        {   
            sendToclient(" need more arguments(operator)\n", socket);
            return;
        }
        if (args[1] == "o" || args[1] == "+o")
        {
            channel.addoperator(args[2]);
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel have one more operator\n");
            return;
        }
        if (args[1] == "-o") 
        {   
            channel.removeoperator(args[2]);
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel have -1 operator\n");
            return;
        }
}

void    mode_i(std::vector<std::string> &args, Channel &channel, std::string nick, int socket)
{
    nick = "";
    if (args.size() < 2)
        {   
            sendToclient(" need more arguments(password)\n", socket);
            return;
        }
        if (args[1] == "i" || args[1] == "+i")
        {
            channel.setInv_mode(true);
            channel.sendToAll("MODE : " + args[1] + " Done, Now channel invite only mode\n");
            return;
        }
        if (args[1] == "-i") 
        {   
            if (args[2].empty())
            {
                channel.setInv_mode(false);
                channel.sendToAll("MODE : " + args[1] + " Done, Now channel is not invite only mode\n");
                return;
            }
        }
}