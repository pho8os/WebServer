/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:05:00 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/11 18:59:36 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

server::server(void) {

}
void	server::set_up( request &set ) { 
	int	_socket_cl, _socket_ser;
	const char *port = "8080";
	struct sockaddr_in server_, client_;
	_socket_ser = socket(AF_INET, SOCK_STREAM, 0);
	server_.sin_family = AF_INET;
	server_.sin_port = htons(atoi(port));
	server_.sin_addr.s_addr = INADDR_ANY;
	bind(_socket_ser, (struct sockaddr *)&server_, sizeof(server_));
	while (1) {
		// std::ofstream	file("output" , std::ios::app);
		char	*buffer = new char[5430];
		listen(_socket_ser, 5);
		std::cout << BOLD_GREEN << "server is listenning at Port " << port << " ->" << DEF << std::endl;
		socklen_t length = sizeof(client_);
		_socket_cl = accept(_socket_ser, (struct sockaddr *)&client_, &length);
		recv(_socket_cl, buffer, 5430, 0);
		// file << buffer;
		std::cout << BOLD_RED << "Request : \n" << DEF << buffer << std::endl;
		set.setBuffer(static_cast<std::string>(buffer));
		if (!set.getBuffer().empty())
			set.HTTPRequest();
		int error_code = set.CheckForBody( set.getBuffer() );
		(error_code == -1 || error_code == -2) && (std::cout << set.FillBody( set.getBuffer(), error_code ) << std::endl), 0;
		const char *response = "Received";
		send(_socket_cl, response, static_cast<std::string>(response).length(), 0);
		set.printVec();
		free(buffer);
	}
	close(_socket_cl);
	close(_socket_ser);
}
server::~server(void) {

}