/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/23 18:03:24 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/26 21:23:53 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cerrno>

//int main()
//{
//	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
//	if (socketfd == -1)
//	{
//		std::cerr << "Failed to create socket :" << errno << std::endl;
//		std::exit(EXIT_FAILURE);
//	}

//	sockaddr_in sockaddr;
//	sockaddr.sin_family = AF_INET;		   // addres family
//	sockaddr.sin_addr.s_addr = INADDR_ANY; // port in byte
//	sockaddr.sin_port = htons(5080);

//	if (bind(socketfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
//	{
//		std::cout << "Failed to bind to port 5080. errno: " << errno << std::endl;
//		exit(EXIT_FAILURE);
//	}

//	int cnx_number = 5;
//	// listen on a max of 5 connections.
//	if (listen(socketfd, cnx_number) < 0)
//	{
//		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
//		exit(EXIT_FAILURE);
//	}
//	// grab a connection from te queque
//	size_t addrlen = sizeof(sockaddr);
//	int connection = accept(socketfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
//	if (connection < 0)
//	{
//		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
//		exit(EXIT_FAILURE);
//	}

//	// read the the request
//	char buffer[100];
//	size_t bytesRead = read(connection, buffer, 100);
//	std::cout << "The message was: " << buffer;

//	// Send a message to the connection
//	std::string response = "hooooooooyah baby !!\n";
//	send(connection, response.c_str(), response.size(), 0);
//	close(connection);
//	close(socketfd);
//	return 0;
//}