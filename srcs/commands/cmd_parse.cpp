/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrouill <atrouill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 10:07:36 by atrouill          #+#    #+#             */
/*   Updated: 2022/05/12 15:43:03 by atrouill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"
#include "commands.hpp"
#include "IRC.Class.hpp"
#include "User.Class.hpp"
#include <vector>
#include <string>


int		cmd_parse ( std::string entry, IRC *serv, User *user )
{
	std::vector<std::string>	commands;

	commands = ft_split(entry, "\r\n");
	while (commands.size())
	{
		debug("Receive %s", commands.front().c_str());
		std::string	tmp = ltrim_copy(commands.front());
		size_t	pos = tmp.find_first_of(' ');

		std::string	cmd, args;

		str_upper(cmd);
		if (pos != std::string::npos)
		{
			cmd = trim_copy(tmp.substr(0, pos));
			args = trim_copy(tmp.substr(pos, tmp.length()));
		}
		else
		{
			cmd = entry;
			args = "";
		}
		if (cmd == "NICK")
		{
			cmd_nick(serv, user, args);
		}
		else if (cmd == "USER")
		{
			cmd_user(serv, user, args);
		}
		commands.erase(commands.begin());
	}
	return (0);
}
