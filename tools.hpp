#ifndef TOOL_HPP
#define TOOL_HPP

// welcome message
# define WELCOME_MSG(nick, hostname, user) ":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!~" + user + "@" + hostname + "\r\n"
#define YOURHOST_MSG(nick, hostname) (std::string(":ft_irc 002 ") + nick + " :Your host is " + hostname + ", running version 1.0\r\n")
# define CREATED_MSG(nick, date) ":ft_irc 003 " + nick + " :This server was created " + date + "\r\n"
# define MYINFO_MSG(nick, userModes, chanModes) ":ft_irc 004 " + nick + " ft_irc 1.0 " + userModes + " " + chanModes + "\r\n"
# define ISUPPORT_MSG(client, tokens) (":ft_irc 005 " + client + " " + tokens " :are available on this server\r\n")

// error message pass and command
# define UNKNOW_COMMAND_MSG(nick, command) ":ft_irc 421 " + nick + " " + command + " :Unknown command\r\n"
# define REQUIRED_MSG(nick, command) (":ft_irc 461 " + nick + " " + command + " :Not enough parameters\r\n")
# define ALREADY_MSG(nick) (":ft_irc 462 " + nick + " :You may not reregister\r\n")
# define PASS_INC_MSG(nick) (":ft_irc 464 " + nick + " :Password incorrect\r\n")

// error message nick
# define NICK_REQUIRED_MSG(nick) ":ft_irc 431 " + nick + " :No nickname given\r\n"
# define NICK_ALREADY_MSG(nick) ":ft_irc 433 " + nick + " :Nickname is already in use\r\n"
# define NICK_INVALID_MSG(nick) ":ft_irc 432 " + nick + " :Erroneous nickname\r\n"
# define NICK_MSG(oldNick, newNick, user, ip) (":" + oldNick + "!~" + user + "@" + ip + " NICK :" + newNick + "\r\n")


// error message join
# define JOIN_NO_SUCH_CHANNEL_MSG(nick, channel) ":ft_irc 403 " + nick + " " + channel + " :No such channel\r\n"
# define JOIN_CANNOT_SEND_TO_CHANNEL_MSG(nick, channel) ":ft_irc 404 " + nick + " " + channel + " :Cannot send to channel\r\n"
# define JOIN_TOO_MANY_CHANNEL_MSG(nick, channel) ":ft_irc 405 " + nick + " " + channel + " :You have joined too many channels\r\n"
# define JOIN_CHANNEL_IS_FULL_MSG(nick, channel) ":ft_irc 471 " + nick + " " + channel + " :Cannot join channel (+l)\r\n"
# define JOIN_BAD_CHANNEL_KEY_MSG(nick, channel) ":ft_irc 475 " + nick + " " + channel + " :Cannot join channel (+k)\r\n"
# define JOIN_BANNED_FROM_CHANNEL_MSG(nick, channel) ":ft_irc 474 " + nick + " " + channel + " :Cannot join channel (+b)\r\n"
# define JOIN_BAD_CHANNEL_MASK_MSG(channel) ":ft_irc 476 " + channel + " :Bad Channel Mask\r\n"
# define JOIN_INVITE_ONLY_CHAN_MSG(nick, channel) ":ft_irc 473 " + nick + " " + channel + " :Cannot join channel (+i)\r\n"
# define JOIN_NOTOPIC_MSG(nick, channel) ":ft_irc 331 " + nick + " " + channel + " :No topic is set\r\n"
# define JOIN_TOPIC_MSG(nick, channel, topic) ":ft_irc 332 " + nick + " " + channel + " :" + topic + "\r\n"
# define JOIN_TOPIC_WHOTIME_MSG(nick, channel, topic, time) ":ft_irc 333 " + nick + " " + channel + " " + topic + " " + time + "\r\n"
# define JOIN_NAMREPLY_MSG(nick, channel, clients) ":ft_irc 353 " + nick + " = " + channel + " :" + clients + "\r\n"
# define JOIN_ENDOFNAMES_MSG(nick, channel) ":ft_irc 366 " + nick + " " + channel + " :End of /NAMES list\r\n"

// error message Kick
# define KICK_CHANOPRIVSNEEDED_MSG(nick, channel) ":ft_irc 482 " + nick + " " + channel + " :You're not channel operator\r\n"
# define KICK_USERNOTINCHANNEL_MSG(user, nick, channel) ":ft_irc 441 " + user + " " + nick + " " + channel + " :They aren't on that channel\r\n"
# define KICK_NOTONCHANNEL_MSG(nick, channel) ":ft_irc 442 " + nick + " " + channel + " :You're not on that channel\r\n"

#endif // TOOL_HPP