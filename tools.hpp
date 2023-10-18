#ifndef TOOL_HPP
#define TOOL_HPP

// welcome message
# define WELCOME_MSG (nick, hostname) ":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!~" + nick + "@" + hostname + "\r\n"
# define YOURHOST_MSG (nick, hostname) ":ft_irc 002 " + nick + " :Your host is " + hostname + ", running version 1.0\r\n"
# define CREATED_MSG (nick, date) ":ft_irc 003 " + nick + " :This server was created " + date + "\r\n"
# define MYINFO_MSG (nick, userModes, chanModes) ":ft_irc 004 " + nick + " ft_irc 1.0 " + userModes + " " + chanModes + "\r\n"
# define ISUPPORT_MSG(client, tokens) (":ft_irc 005 " + client + " " + tokens " :are available on this server\r\n")

// error message pass and command
# define UNKNOWN_COMMAND_MSG(command) ":ft_irc 421  " + command + " :Unknown command\r\n"
# define REQUIRED_MSG (nick) ":ft_irc 461 " + nick + " PASS :Not enough parameters\r\n"
# define ALREADY_MSG (nick) ":ft_irc 462 " + nick + " :You may not reregister\r\n"
# define PASS_INC_MSG (nick) ":ft_irc 464 " + nick + " :Password incorrect\r\n"

// error message nick
# define NICK_REQUIRED_MSG (nick) ":ft_irc 431 " + nick + " :No nickname given\r\n"
# define NICK_ALREADY_MSG (nick) ":ft_irc 433 " + nick + " :Nickname is already in use\r\n"
# define NICK_INVALID_MSG (nick) ":ft_irc 432 " + nick + " :Erroneous nickname\r\n"


#endif // TOOL_HPP
