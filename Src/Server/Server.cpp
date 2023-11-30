

#include "Server.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <map>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>
#include <ostream>


std::string file_gen()
{
  static size_t i;
  return (std::string("/tmp/file_") + std::to_string(i++));
}

//bool check_done(const std::string &req)
//{
//  return false;
//}

bool MServer::port_exist(size_t &index) const
{
  for(size_t i = 0; i < index; i++)
    if(servers[i].listen == servers[index].listen)
    { 

      std::cout << servers[index].listen.first; 
      return true;
      
      }
  return false;
}


MServer::MServer() : servers(Config::getConfig())
{
  nserv = Config::getConfig().size();
}

MServer::~MServer()
{
} 

void MServer::Serving()
{
  for(size_t i = 0; i < nserv; i++)
  {
    if(!port_exist(i))
    {
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      if(sock < 0)
        throw std::runtime_error("Server: creating socket failed");
      sockaddr_in addrserv[nserv];
      addrserv[i].sin_family = AF_INET;
      addrserv[i].sin_addr.s_addr = INADDR_ANY;
      addrserv[i].sin_port = htons(std::atoi(servers[i].listen.second.c_str()));
      if (bind(sock, (struct sockaddr *)(&addrserv[i]), sizeof(addrserv[i])) == -1)
             throw(std::runtime_error("Server: Socket failed to bind"));
      if (listen(sock, SOMAXCONN) == -1)
        throw(std::runtime_error("Server: Listening failed"));
      struct pollfd a;
      bzero(&a, sizeof(pollfd));
      a.fd = sock;
      a.events = POLLIN;
      fds.push_back(a);
      servfd.push_back(sock);
    }
  }
  this->run();
}

void MServer::receiving(const size_t &index)
{
  if(index < servfd.size())
  {
    int client = accept(fds[index].fd, NULL, NULL);
    if (client == -1)
      std::cerr << "accept: A client refused to connect" << std::endl;
    struct pollfd a;
    bzero(&a, sizeof(pollfd));
    a.fd = client;
    a.events = POLLIN;
    fds.push_back(a);
    reqs[client].push_back("");
  }
  else {
    char *data = new char[PAGE];
    ssize_t re = recv(fds[index].fd, data, PAGE, 0);
    write(1, data, re);
    fds[index].events = POLLOUT;
    return ;
  }
}

void MServer::run()
{
  while(true)
  {
    int timeout = (fds.size() == nserv) * -1 + (fds.size() > nserv) * 6000;
    poll(&fds[0], fds.size(), timeout);
    for(size_t i = 0;  i < fds.size(); i++)
    {
      if(fds[i].revents & POLLHUP)
      {
        close(fds[i].fd);
        fds.erase(fds.begin() + i );
      }
      else if(fds[i].revents & POLLIN)
      {
        this->receiving(i);
                std::string http_response = "HTTP/1.1 200 OK\r\nServer: MyWebServer/1.0\r\nContent-Type: text/html\r\nContent-Length: 123\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n    <title>Sample Page</title>\r\n</head>\r\n<body>\r\n    <h1>Hello, World!</h1>\r\n    <p>This is a sample HTTP/1.1 response.</p>\r\n</body>\r\n</html>\r\n";
        send(fds[i].fd, http_response.c_str(), http_response.length(), -1);

        break;
      }
      else if(fds[i].revents & POLLOUT)
      {
        std::cout << "sending" << std::endl;
        std::string http_response = "HTTP/1.1 200 OK\r\nServer: MyWebServer/1.0\r\nContent-Type: text/html\r\nContent-Length: 123\r\n\r\n<!DOCTYPE html>\r\n<html>\r\n<head>\r\n    <title>Sample Page</title>\r\n</head>\r\n<body>\r\n    <h1>Hello, World!</h1>\r\n    <p>This is a sample HTTP/1.1 response.</p>\r\n</body>\r\n</html>\r\n";
        send(fds[i].fd, http_response.c_str(), http_response.length(), -1);
        fds[i].events = POLLIN;
      }
    }
  }
}
