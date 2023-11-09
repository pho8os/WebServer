/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:21:43 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/09 12:29:17 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 11:01:21 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/09 11:19:48 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read

int main()
{
    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 8080 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    int port = 8080;
    sockaddr.sin_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cout << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "Server Listening on port : " << port << std::endl;
    std::cout << "===============================" << std::endl;

    // Start listening. Hold at most 10 connections in the queue
    if (listen(sockfd, 10) < 0)
    {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        // Grab a connection from the queue
        size_t addrlen = sizeof(sockaddr);
        int connection = accept(sockfd, (struct sockaddr *)&sockaddr, (socklen_t *)&addrlen);
        if (connection < 0)
        {
            std::cout << "Failed to grab connection. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        // Read from the connection
        char buffer[100024];
        size_t bytesRead = read(connection, buffer, sizeof(buffer));

        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the received data

            if (strcmp(buffer, "---terminate---") == 0) {
                std::cout << "Termination signal received. Closing connection and server." << std::endl;
                close(connection);
                close(sockfd);
                break; // Exit the loop
            } else {
                std::cout << "Received: " << buffer << std::endl;

                // Send a message to the connection
                std::string response = "Good talking to you\n";
                send(connection, response.c_str(), response.size(), 0);
            }
            std::cout << "------------>+++<------------" << std::endl;
        }

        // Close the connection
        close(connection);
    }

    return 0;
}