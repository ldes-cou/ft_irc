/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TCPServer.Class.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atrouill <atrouill@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 16:50:26 by atrouill          #+#    #+#             */
/*   Updated: 2022/05/23 19:07:05 by atrouill         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_CLASS_HPP
# define TCPSERVER_CLASS_HPP

# include "config.hpp"
# include "User.Class.hpp"

# include <netdb.h>
# include <netinet/tcp.h>
# include <fcntl.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/time.h>
# include <algorithm>
# include <cstring>
# include <unistd.h>
# include <cerrno>
# include <map>

class TCPServer
{
	private:
		struct sockaddr_in				_address;
		std::string						_hostname;
		int								_clients_socket[MAX_CLIENTS_CONNECTION];
		fd_set							_listen_socket;
		int								_main_socket;
		std::multimap<int, std::string>	_buffer_out;

	public:
		TCPServer ( int port = PORT );

		void						pending_activity ( void );

		std::pair<int, std::string>	incoming_connection ( void );

		std::pair<int, std::string>	receive_data ( void );

		void						add_to_buffer ( std::pair<int, std::string> buff );

		void						send_buffer ( void );

		const std::string &			getHostname ( void ) const;

		void						close_connection ( const int & fd );

};

#endif
