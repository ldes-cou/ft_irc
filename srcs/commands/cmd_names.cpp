/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_names.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrouill <atrouill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 18:27:31 by atrouill          #+#    #+#             */
/*   Updated: 2022/05/17 10:33:06 by atrouill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPL.hpp"
#include "utils.hpp"
#include "IRC.Class.hpp"
#include "commands.hpp"
#include "functions.hpp"
#include <vector>
#include <string>

void	cmd_names ( IRC *serv, User *user, std::string & args )
{
	std::string								msg;
	std::map<User *, bool>::iterator		user_it;
	std::map<User *, bool>					connected_users;
	
	if (args != "")
	{
		std::vector<std::string>				chan;
		std::pair<bool, Channel *>				res;
		
		chan = ft_split(args, ",");
		for (size_t i = 0; i < chan.size(); i++)
		{
			res = serv->get_channel(chan[i]);
			if (res.first)
			{
				connected_users = res.second->getUsers();
				user_it = connected_users.begin();
				msg = send_rpl(353, serv, user, chan[i]);
				while (user_it != connected_users.end())
				{
					if (user_it->second)
						msg.append("@");
					else
						msg.append(" ");
					msg.append(user_it->first->_nick_name);
					msg.append(" ");
					user_it++;
				}
				msg.append("\r\n");
				serv->_tcp.add_to_buffer(std::make_pair(user->_fd, msg));
				serv->_tcp.add_to_buffer(std::make_pair(user->_fd, send_rpl(366, serv, user, chan[i])));
			}			
		}
	}
	else
	{
		std::map<std::string, Channel*>				chan;
		std::map<std::string, Channel*>::iterator	chan_it;

		chan = serv->get_channel();
		chan_it = chan.begin();
		while (chan_it != chan.end())
		{
			connected_users = chan_it->second->getUsers();
			user_it = connected_users.begin();
			msg = send_rpl(353, serv, user, chan_it->first);
			while (user_it != connected_users.end())
			{
				if (user_it->second)
					msg.append("@");
				else
					msg.append(" ");
				msg.append(user_it->first->_nick_name);
				msg.append(" ");
				user_it++;
			}
			msg.append("\r\n");
			serv->_tcp.add_to_buffer(std::make_pair(user->_fd, msg));
			serv->_tcp.add_to_buffer(std::make_pair(user->_fd, send_rpl(366, serv, user, chan_it->first)));
			chan_it++;
		}	
	}
}