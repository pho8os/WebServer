/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:24:05 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/16 12:47:22 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::string content = "<h1><center>Welcome</center></h1>";

// Construct HTTP response

Server::Server(int port) : _port(port)
{
    _max_clients = 2;
    _buffer_size = 256000;
    struct pollfd _fds[_max_clients + 1];
    bzero(_fds, sizeof(_fds));

    resp << "HTTP/1.1 200 OK\r\n";
    resp << "Content-Type: text/html\r\n";
    resp << "Content-Length: " << content.length() << "\r\n";
    resp << "\r\n";
    resp << content;
}

bool Server::initServer()
{
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket == -1)
        return (std::cerr << "Error creating server socket\n" && 1);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(_port);

    if (bind(_serverSocket, (struct sockaddr *)(&serverAddr), sizeof(serverAddr)) == -1)
        return (std::cerr << "Error binding server socket\n" && close(_serverSocket), false);

    if (listen(_serverSocket, _max_clients) == -1)
        return (std::cerr << "Error listening on server socket\n" && close(_serverSocket), false);
    std::cout << "Server listening on port  " << _port << "...\n";
    std::cout << "=================================\n";

    this->run();
    close(_serverSocket);
    return true;
}

void Server::run()
{
    struct pollfd _fds[_max_clients + 1];
    bzero(_fds, sizeof(_fds));
    _fds[0].fd = _serverSocket;
    _fds[0].events = POLLIN;
    while (true)
    {
        int result = poll(_fds, _clients.size() + 1, -1);

        if (result == -1)
        {
            std::cerr << "Error in poll\n";
            break;
        }
        if (_fds[0].revents & POLLIN)
        {
            int clientSocket = accept(_serverSocket, nullptr, nullptr);
            if (clientSocket != -1)
            {
                if (_clients.size() < (size_t)_max_clients)
                {
                    _clients.push_back(clientSocket);
                    _fds[_clients.size()].fd = clientSocket;
                    _fds[_clients.size()].events = POLLIN;
                    _incompleteRequests.push_back("");
                    std::cout << _GREEN << "New client connected. Total clients: " << _RESET << _clients.size() << "\n";
                }
                else
                {
                    std::cout << "Connection limit reached. Rejecting new connection.\n";
                    close(clientSocket);
                }
            }
        }
        for (size_t i = 1; i <= _clients.size(); ++i)
        {
            if (_fds[i].revents & POLLIN)
            {
                char *buffer = NULL;
                buffer = (char *)malloc(_buffer_size);

                int bytesRead = recv(_fds[i].fd, buffer, _buffer_size, 0);
                std::cout << "Buffer Len  : " << std::strlen(buffer) << std::endl;

                if (bytesRead <= 0)
                {
                    close(_fds[i].fd);
                    std::cout << _RED << "Client disconnected. Total clients: " << _RESET << _clients.size() - 1 << "\n";
                    _clients.erase(_clients.begin() + i - 1);
                    _incompleteRequests.erase(_incompleteRequests.begin() + i - 1);
                    _fds[i] = _fds[_clients.size() + 1];
                    --i;
                }
                else
                {
                    _incompleteRequests[i - 1] += std::string(buffer, bytesRead);

                    size_t found = _incompleteRequests[i - 1].find("\r\n\r\n");
                    if (found != std::string::npos)
                    {
                        _fillHeader(_incompleteRequests[i - 1]);
                        fill_file(_incompleteRequests[i - 1], std::string("file"));
                        send(_fds[i].fd, (resp.str()).c_str(), resp.str().length(), 0);
                        _incompleteRequests[i - 1].clear();
                    }
                }
            }
        }
    }
}

void Server::fill_file(std::string str, std::string name)
{
    std::ofstream ofile(name.c_str());

    if (!ofile.is_open() || ofile.bad())
        throw std::runtime_error("Error : Cann't open file !!");

    ofile << str;
    if (ofile.bad())
    {
        ofile << std::endl;
        ofile.close();
        throw std::runtime_error("Error : Error dealing with files !!");
    }
    ofile << std::endl;
    ofile.close();
}

bool Server::_fillHeader(st_ request_)
{
    std::cout << request_ << std::endl
              << "++++++++" << std::endl;
    for (int i = 0; request_.substr(0, 2) != "\r\n" && !request_.empty(); i++)
    {
        size_t found_it = request_.find(": ");
        if (found_it != std::string::npos)
        {
            st_ key = request_.substr(0, found_it);
            request_.erase(0, found_it + 2);
            size_t found_end = request_.find("\r\n");
            if (found_end == std::string::npos || key.empty())
                break;
            st_ value = request_.substr(0, found_end);
            request_.erase(0, found_end + 2);
            headers.push_back(std::make_pair(key, value));
        }
        else
            return perror("MetaData Error\n"), Parsed = false, false;
    }
    if (!CheckForBody(request_))
        return Parsed = false, false;
    return true;
}

int Server::CheckForBody(st_ request_)
{
    std::vector<std::pair<st_, st_> >::iterator it_ = headers.begin();
    for (; it_ != headers.end(); it_++)
    {
        if ((!it_->first.compare("Content-Length")) || (!it_->first.compare("Transfer-Encoding")))
        {
            if ((!it_->first.compare("Content-Length") && atoi(it_->second.c_str()) <= 0 && !getMethod_().compare("POST")) || ((int)body.length() > atoi(it_->second.c_str()) && !getMethod_().compare("POST")))
                return perror("400 Bad Request\n"), 0;
            else if (!it_->first.compare("Transfer-Encoding") && it_->second.compare("chunked"))
                return perror("501 Not Implimented\n"), 0;
            request_.erase(0, request_.find("\r\n") + 2);
            setBody(request_);
            break;
        }
    }
    if (it_ == headers.end() && !getMethod_().compare("POST"))
        return perror("400 Bad Request\n"), 0;
    return 1;
}
void Server::setBody(std::string body)
{
    this->body = body;
}

std::string &Server::getMethod_(void)
{
    return Method_;
}

Server::~Server()
{
}