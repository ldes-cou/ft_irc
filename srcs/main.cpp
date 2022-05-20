/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrouill <atrouill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:39:29 by atrouill          #+#    #+#             */
/*   Updated: 2022/05/19 21:40:29 by atrouill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"
#include "commands.hpp"
#include "utils.hpp"
#include "IRC.Class.hpp"
#include <map>

void	loop (IRC *server)
{
	std::pair<int, std::string>		in_connection;
	std::pair<int, std::string>		buffer;
	std::map<int, User>::iterator	it;

	while (true)
	{
		// std::cout  << std::endl << std::endl << "PENDING" << std::endl;
		server->_tcp.pending_activity();
		// std::cout << "INCOMING" << std::endl;
		in_connection = server->_tcp.incoming_connection();
		if (in_connection.first != 0)
		{
			server->add_user(in_connection.first, in_connection.second);
			if (server->get_password() == "")
			{
				server->get_user(in_connection.first)->_pass_send = true;
			}
		}
		// std::cout << "RECEIVE" << std::endl;
		buffer = server->_tcp.receive_data();
		if (buffer.first != 0)
		{
			if (buffer.second == "Disconnected\n")
			{
				server->remove_user(buffer.first);
			}
			else
			{
				cmd_parse(buffer.second, server, server->get_user(buffer.first));
			}
		}
		// std::cout << "SENDING" << std::endl;
		server->_tcp.send_buffer();
	}
}

int	main(int argc, char **argv)
{
	std::string	password;
	int			port;

	port = (argc >= 2) ? std::atoi(argv[1]) : PORT;
	password = (argc == 3) ? argv[2] : "";

	IRC	server(port, password);

	loop (&server);

	return (0);
}
