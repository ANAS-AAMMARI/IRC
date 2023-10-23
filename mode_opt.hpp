#ifndef MODE_OPT_HPP
# define MODE_OPT_HPP

#include "server.hpp"
#include "channel.hpp"
#include "channel.hpp"

class Channel;

void    sendToclient(const std::string &msg, int clientSocket);

// option of MODE COMMAND
void    mode_k(std::vector<std::string> &args, Channel &channel, std::string nick, int socket);
void    mode_l(std::vector<std::string> &args, Channel &channel, std::string nick, int socket);
void    mode_o(std::vector<std::string> &args, Channel &channel, std::string nick, int socket);
void    mode_i(std::vector<std::string> &args, Channel &channel, std::string nick, int socket);

#endif