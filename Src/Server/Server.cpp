

#include "Server.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <map>
#include <netdb.h>
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
      return true;
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
      servfd.push_back(socket(AF_INET, SOCK_STREAM, 0));
      if(servfd[i] == -1)
        throw std::runtime_error("Server: creating socket failed");
      sockaddr_in addrserv[nserv];
      addrserv[i].sin_family = AF_INET;
      addrserv[i].sin_addr.s_addr = INADDR_ANY;
      addrserv[i].sin_port = htons(std::atoi(servers[i].listen.second.c_str()));
      if (bind(servfd[i], (struct sockaddr *)(&addrserv[i]), sizeof(addrserv[i])) == -1)
             throw(std::runtime_error("Server: Socket failed to bind"));
      if (listen(servfd[i], MAX_CLENTS) == -1)
        throw(std::runtime_error("Server: Listening failed"));
      struct pollfd a;
      bzero(&a, sizeof(pollfd));
      a.fd = servfd[i];
      a.events = POLLIN;
      fds.push_back(a);
    }
  }
  this->run();
}

void MServer::receiving(const size_t &index)
{
  if(index < nserv)
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
    recv(fds[index].fd, data, PAGE, 0);
    //std::freopen("file", "w+", stdout);
    //std::cout << data;
    std::cout << data;
    //reqs[fds[index].fd] += std::string(data, ret);
  }
}

void MServer::run()
{
  while(true)
  {
    int timeout = (fds.size() == nserv) * -1 + (fds.size() > nserv) * 60000;
    poll(&fds[0], fds.size(), timeout);
    for(size_t i = 0;  i < fds.size(); i++)
    {
      if(fds[i].revents & POLLIN)
        this->receiving(i);
      else if(fds[i].revents & POLLOUT)
        ;
    }
  }
}
