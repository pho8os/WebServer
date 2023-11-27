/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:05:27 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/27 18:02:08 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sstream>
#include <fstream>
#include "../cgi/Cgi.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include <cstdio>
#include <exception>
#include <sys/fcntl.h>

#define _RED "\033[1;31m"
#define _GREEN "\033[1;32m"
#define _RESET "\033[0m"

#define st_ std::string
#define Vect std::vector<std::pair<st_, st_> >

class MServer
{
private:
    bool Parsed;
    st_ Method_;
    int _serverSocket;
    int _max_clients;
    int _buffer_size;
    int _port;
    std::vector<int> _clients;
    Vect headers;
    std::vector<std::string> _incompleteRequests;
    st_ body;

    std::ostringstream resp;

public:
    MServer();
    MServer(int port);
    void run();
    void acceptClient();
    bool initServer();
    void setBody(std::string body);
    std::string &getMethod_(void);
    void fill_file(std::string str, std::string name);
    ~MServer() {};
    bool _fillHeader(std::string request_);
    int CheckForBody(st_ request_);
};