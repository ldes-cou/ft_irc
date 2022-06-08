/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_notice.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldes-cou <ldes-cou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/24 14:15:26 by ldes-cou          #+#    #+#             */
/*   Updated: 2022/06/08 16:22:25 by ldes-cou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPL.hpp"
#include "utils.hpp"
#include "commands.hpp"
#include "IRC.Class.hpp"
#include "User.Class.hpp"
#include "functions.hpp"
#include <vector>
#include <string>

static bool		valid_args ( IRC *serv, User *user, std::string & args )
{
	std::vector<std::string>	split;
	std::string					target, message;

	if (args.find_first_of(':') != std::string::npos)
	{
		split = ft_split(args, ":");
		target = trim_copy(split[0]);
		if (target == "")
		{
			serv->_tcp.add_to_buffer(std::make_pair(
				user->_fd,
				send_rpl(411, serv, user, "PRIVMSG")
			));
			return (false);
		}
		message = trim_copy(split[1]);
		if (message == "")
		{
			serv->_tcp.add_to_buffer(std::make_pair(
				user->_fd,
				send_rpl(412, serv, user)
			));
			return (false);
		}
		return (true);
	}
	serv->_tcp.add_to_buffer(std::make_pair(
		user->_fd,
		send_rpl(412, serv, user)));
	return (false);
}

void	cmd_notice (IRC *serv, User *user, std::string & args)
{
	std::vector<std::string>	split;
	std::string					target, message;


	if (args.find_first_of(':') != std::string::npos)
	{
		split = ft_split(args, ":");
		if (split.size() != 0)
			target = trim_copy(split[0]);
		else
			return;
	}
	else
		return;
	if (valid_args(serv, user, args))
	{
		message.append(user_answer(user));
		message.append("NOTICE ");
		message.append(target);
		message.append(" :");
		message.append(split[1]);
		message.append("\r\n");
		if (target.find_first_of(CHAN_FIRST, 0) == std::string::npos)
		{
			debug("Send message to user");
			std::pair<bool, User*>	receiver;

			receiver = serv->get_user(target);
			if (receiver.first)
				serv->_tcp.add_to_buffer(std::make_pair(receiver.second->_fd, message));				
		}
		else
		{
			debug("Send message to channel");
			std::pair<bool, Channel*>	receiver;

			receiver = serv->get_channel(target);
			if (receiver.first)
			{
				if (receiver.second->userIsIn(user))
					receiver.second->send(serv, user, message);
			}
		}
		
	}
}