/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:05:00 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/19 14:38:52 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

server::server(void) {

}
void	server::set_up( request &set, Response &res ) { 
	int	_socket_cl, _socket_ser;
	int port = 8080;
	struct sockaddr_in server_, client_;
	_socket_ser = socket(AF_INET, SOCK_STREAM, 0);
	server_.sin_family = AF_INET;
	server_.sin_port = htons(port);
	server_.sin_addr.s_addr = INADDR_ANY;
	bind(_socket_ser, (struct sockaddr *)&server_, sizeof(server_));
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
	res = res.RetResponse(set);
	std::cout << "|" << res.getRet() << "|" << std::endl;
	send(_socket_cl, res.getRet().c_str(), res.getRet().length(), 0);
	close(_socket_cl);
	close(_socket_ser);
	set.printVec();
	free(buffer);
}
server::~server(void) {

}