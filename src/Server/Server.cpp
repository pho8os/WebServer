#include "Server.hpp"
#include <ostream>
#include <sstream>


std::string content = "<h1><center>Welcome</center></h1>";


// Construct HTTP response

    resp << "HTTP/1.1 200 OK\r\n";
    resp << "Content-Type: text/html\r\n";
    resp << "Content-Length: " << content.length() << "\r\n";
    resp << "\r\n";
    resp << content;
MServer::MServer(int port) : _port(port) {
  _max_clients = 2;
  _buffer_size = 256000;
  struct pollfd _fds[_max_clients + 1];
  bzero(_fds, sizeof(_fds));
}

bool MServer::initServer() {
  _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (_serverSocket == -1)
    return (std::cerr << "Error creating server socket\n" && 1);

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(_port);

  if (bind(_serverSocket, (struct sockaddr *)(&serverAddr),
           sizeof(serverAddr)) == -1)
    return (std::cerr << "Error binding server socket\n" &&
                close(_serverSocket),
            false);

  if (listen(_serverSocket, _max_clients) == -1)
    return (std::cerr << "Error listening on server socket\n" &&
                close(_serverSocket),
            false);
  std::cout << "Server listening on port  " << _port << "...\n";
  std::cout << "=================================\n";

  this->run();
  close(_serverSocket);
  return true;
}

void MServer::run() {
  Response res;
  struct pollfd _fds[_max_clients + 1];
  bzero(_fds, sizeof(_fds));
  _fds[0].fd = _serverSocket;
  _fds[0].events = POLLIN;
  while (true) {
    int result = poll(_fds, _clients.size() + 1, -1);

    if (result == -1) {
      std::cerr << "Error in poll\n";
      break;
    }
    if (_fds[0].revents & POLLIN) {
      int clientSocket = accept(_serverSocket, nullptr, nullptr);
      if (clientSocket != -1) {
        if (_clients.size() < (size_t)_max_clients) {
          _clients.push_back(clientSocket);
          _fds[_clients.size()].fd = clientSocket;
          _fds[_clients.size()].events = POLLIN;
          _incompleteRequests.push_back("");
          std::cout << _GREEN
                    << "New client connected. Total clients: " << _RESET
                    << _clients.size() << "\n";
        } else {
          std::cout << "Connection limit reached. Rejecting new connection.\n";
          close(clientSocket);
        }
      }
    }
    for (size_t i = 1; i <= _clients.size(); ++i) {
      if (_fds[i].revents & POLLIN) {
        char *buffer = NULL;
        buffer = (char *)malloc(_buffer_size);

        int bytesRead = recv(_fds[i].fd, buffer, _buffer_size, 0);
        if (bytesRead <= 0) {
          close(_fds[i].fd);
          std::cout << _RED << "Client disconnected. Total clients: " << _RESET
                    << _clients.size() - 1 << "\n";
          _clients.erase(_clients.begin() + i - 1);
          _incompleteRequests.erase(_incompleteRequests.begin() + i - 1);
          _fds[i] = _fds[_clients.size() + 1];
          --i;
        } else {
          try {
            request req(buffer);
            Cgi a;
            a.setEnvVars(req.getVector());

          } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
          }
        //   res = res.RetResponse(req);
              std::string http_response = "HTTP/1.1 200 OK\r\nServer: MyWebServer/1.0\r\nContent-Type: text/html\r\nContent-Length: 123\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n    <title>Sample Page</title>\r\n</head>\r\n<body>\r\n    <h1>Hello, World!</h1>\r\n    <p>This is a sample HTTP/1.1 response.</p>\r\n</body>\r\n</html>\r\n";

    // Send the response to the client
        //   send(_fds[i].fd, res.getRet().c_str(), res.getRet().length(), 0);
        send(_fds[i].fd, http_response.c_str(), http_response.size(), 0);
        }
      }
    }
  }
}