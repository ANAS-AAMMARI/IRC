#ifndef MODE_OPT_HPP
# define MODE_OPT_HPP

#include "server.hpp"
#include "channel.hpp"

class Client;
class Channel;

void    sendToclient(const std::string &msg, int clientSocket);

void    mode_k(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg);
void    mode_l(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg);
void    mode_o(std::vector<std::string> &args, Channel &channel, Client client, bool &is_munis, size_t &count, std::string &msg);
void    mode_i(Channel &channel, Client client, bool &is_munis, std::string &msg);
void    mode_tp(Channel &channel, Client client, bool &is_munis, std::string &msg);

#endif 
