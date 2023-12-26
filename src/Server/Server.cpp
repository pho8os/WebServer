#include "Server.hpp"
#include <cstddef>
#include <strings.h>
#include <sys/_types/_off_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

std::string file_gen()
{
  static size_t i;
  return (std::string("/tmp/file_") + std::to_string(i++));
}

bool MServer::port_exist(size_t &index) const
{
  for (size_t i = 0; i < index; i++)
    if (servers[i].listen == servers[index].listen)
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

int guard(int n, std::string err)
{
  if (n == -1)
  {
    throw std::runtime_error(err);
  }
  return n;
}

void MServer::Serving()
{
  for (size_t i = 0; i < nserv; i++)
  {
    if (!port_exist(i))
    {
      int option = 1;
      int sock = guard(socket(AF_INET, SOCK_STREAM, 0), "could not create TCP listening socket");
      int flags = guard(fcntl(sock, F_GETFL), "could not get flags on TCP listening socket");
      guard(fcntl(sock, F_SETFL, flags | O_NONBLOCK), "could not set TCP listening socket to be non-blocking");
      int x = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(option));
      if (sock < 0)
        throw std::runtime_error("Server: creating socket failed");
      sockaddr_in addrserv[nserv];
      addrserv[i].sin_family = AF_INET;
      addrserv[i].sin_addr.s_addr = htonl(INADDR_ANY);
      addrserv[i].sin_port = htons(std::atoi(servers[i].listen.second.c_str()));
      guard(bind(sock, (struct sockaddr *)(&addrserv[i]), sizeof(addrserv[i])), "Server: Socket failed to bind");
      guard((listen(sock, SOMAXCONN) == -1), "Server: Listening failed");
      struct pollfd a;
      bzero(&a, sizeof(pollfd));
      a.fd = sock;
      a.events = POLLIN;
      lstPoll.push_back(a);
      servfd.push_back(sock);
    }
  }
  this->run();
}

void MServer::sending(const size_t &index)
{
  off_t len = PAGE;
  Response &obj = clientsData[lstPoll[index].fd].second;
  if (obj.sending)
  {
    if (!obj.headersent)
    {
      std::string &str = obj.getRet();
      if (str.size() >= PAGE)
      {
        ssize_t re = send(lstPoll[index].fd, str.c_str(), PAGE, 0);
        str = str.erase(0, PAGE);
        return;
      }
      else
      {
        ssize_t re = send(lstPoll[index].fd, str.c_str(), str.size(), 0);
        obj.headersent = true;
        if (obj.getFd() == -1)
        {
          obj.sending = false;
          return;
        }
        len -= re;
      }
    }
    if (obj.getFd() != -1)
    {
      char *data = new char[len];
      ssize_t re = read(obj.getFd(), data, len);
      if (!re)
        return (obj.sending = false, (void)0);
      ssize_t res = send(lstPoll[index].fd, data, re, 0);
      delete[] data;
    }
  }
  if (!obj.sending)
  {
    bool var = clientsData[lstPoll[index].fd].first.getConnection();
    if(var)
      clientsData[lstPoll[index].fd].first.clear_Obj();
    else
    {
      clientsData.erase(lstPoll[index].fd);
      close(lstPoll[index].fd);
      lstPoll.erase(lstPoll.begin() + index);
    }
  }
}

void MServer::acceptClient(const size_t &index)
{
  int client = accept(lstPoll[index].fd, NULL, NULL);
  if (client == -1)
    std::cerr << "accept: A client refused to connect" << std::endl;
  struct pollfd a;
  bzero(&a, sizeof(pollfd));
  a.fd = client;
  a.events = POLLIN;
  lstPoll.push_back(a);
  request req;
  Response res;
  clientsData[client] = std::make_pair(req, res);
}

void MServer::handleClient(const size_t &index)
{
  int fd = lstPoll[index].fd;
  char *data = new char[PAGE];

  ssize_t re = recv(fd, data, PAGE, 0);

  if (clientsData[fd].first.reading && re > 0)
    clientsData[fd].first.feedMe(std::string(data, re));
  if (!clientsData[fd].first.reading)
  {
    lstPoll[index].events = POLLOUT;
    clientsData[fd].second.RetResponse(clientsData[fd].first);
  }
  delete[] data;
}

void MServer::receiving(const size_t &index)
{
  if (index < servfd.size())
    acceptClient(index);
  else
    handleClient(index);
}

void MServer::run()
{
  while (true)
  {
    int timeout = (lstPoll.size() == nserv) * -1 + (lstPoll.size() > nserv) * 6000;
    int x = poll(&lstPoll[0], lstPoll.size(), timeout);
    if(x == -1)
      continue;
    if(!x)
    {
      for(size_t i = servfd.size(); i < lstPoll.size(); i++)
      {
        close(lstPoll[i].fd);
        lstPoll.erase(lstPoll.begin() + i);
      }
    }      
    for (size_t i = 0; i < lstPoll.size(); i++)
    {
      if (lstPoll[i].revents & POLLIN)
        this->receiving(i);
      else if (lstPoll[i].revents & POLLOUT)
        this->sending(i);
      else if (lstPoll[i].revents & POLL_HUP)
      {
        std::cout << "Clients :: haahua mcha !! \n";
      }
    }
  }
}
