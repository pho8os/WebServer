

#include "Server.hpp"
#include <arpa/inet.h>
#include <map>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>


bool check_done(const std::string &req)
{
  return false;
}

bool MServer::port_exist(size_t &index) const
{
  for(size_t i = 0; i < index; i++)
    if(servers[i].listen == servers[index].listen)
      return true;
  return false;
}


MServer::MServer()
{
  servers = Config::getConfig();
  nserv = Config::getConfig().size();
  addrserv = new struct addrinfo[nserv];
}

MServer::~MServer()
{
  delete [] addrserv;
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
      struct addrinfo hints;
      struct addrinfo *ser;
      bzero(&hints, sizeof(hints));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      if(getaddrinfo(servers[i].listen.first.c_str(), servers[i].listen.second.c_str(), &hints, &ser) == -1)
        throw  std::runtime_error("Server: getaddrinfo failed");
      if (bind(servfd[i], ser->ai_addr, ser->ai_addrlen) == -1)
        throw(std::runtime_error("Server: Socket failed to bind"));
      freeaddrinfo(ser);
      if (listen(servfd[i], SOMAXCONN) == -1)
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
    reqs[client] = "";
  }
  else {
    char *data = new char[PAGE];
    int ret = recv(fds[index].fd, data, PAGE, 0);
    reqs[fds[index].fd] += std::string(data, ret);

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

//void MServer::run() {
//  unsigned int nserv = this->conf.getConfig().size();
//  sockaddr_in addrserv[nserv];
//  std::vector<int> servfd;
//  std::map<int, std::string> reqs;
//  for (size_t i = 0; i < nserv; i++) {
//    if(this->port_exist(i))
//      continue;
//    servfd.push_back(socket(AF_INET, SOCK_STREAM, 0));
//    addrserv[i].sin_family = AF_INET;
//    addrserv[i].sin_addr.s_addr = INADDR_ANY;
//    addrserv[i].sin_port = htons(this->conf.getConfig()[i].listen.second);
//    if (bind(servfd[i], (struct sockaddr *)(&addrserv[i]),
//             sizeof(addrserv[i])) == -1)
//      throw(std::runtime_error("Server: Socket failed to bind"));
//    if (listen(servfd[i], MAX_CLENTS) == -1)
//      throw(std::runtime_error("Server: Listening failed"));
//  }

//  std::vector<struct pollfd> fds;
//  while (true) {
//    poll(&fds[0], fds.size(), -1);
//    for (size_t i = 0; i < fds.size(); i++) {
//      if (fds[i].revents & POLLIN) {
//        if (i < nserv) {
//          int client = accept(fds[i].fd, NULL, NULL);
//          if (client == -1)
//            std::cerr << "accept: A client refused to connect" << std::endl;
//          struct pollfd a;
//          bzero(&a, sizeof(pollfd));
//          a.fd = client;
//          a.events = POLLIN;
//          fds.push_back(a);
//          reqs[client] = "";
//        } else {
//          if (fds[i].revents & POLLIN) {
//            char *data = new char[PAGE];
//            int ret = recv(fds[i].fd, data, PAGE, 0);
//            reqs[fds[i].fd] += std::string(data, ret);
//            if(!ret && check_done(reqs[fds[i].fd]))
//            {
              
//            }

//            delete[] data;
//          }
//        }
//      }
//      if (i > nserv && fds[i].revents & POLLOUT) {
		
//      }
//    }
//  }
//}