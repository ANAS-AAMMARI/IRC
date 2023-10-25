#ifndef MODE_OPT_HPP
# define MODE_OPT_HPP

#include "server.hpp"
#include "channel.hpp"

class Client;
class Channel;

void    sendToclient(const std::string &msg, int clientSocket);

// option of MODE COMMAND
void    mode_i(Channel &channel, Client client, bool &is_munis);
void    mode_k(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count);
void    mode_l(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count);
void    mode_o(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count);

//void    mode_t(std::vector<std::string> &args, Channel &channel, std::string nick, int socket);
#endif 
//mode #te ik-lo ss 1 bimo