

#include "Server.hpp"
#include <arpa/inet.h>
#include <map>
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

bool MServer::port_exist(const Server &serv) const
{
  std::vector<Server> server = this->conf.getConfig();
  for(size_t i = 0; )
  return false;
}

void MServer::run() {
  unsigned int nserv = this->conf.getConfig().size();
  sockaddr_in addrserv[nserv];
  std::vector<int> servfd;
  std::map<int, std::string> reqs;
  for (size_t i = 0; i < nserv; i++) {
    if(port_exist(this->conf.getConfig()[i]))
      continue;
    servfd.push_back(socket(AF_INET, SOCK_STREAM, 0));
    addrserv[i].sin_family = AF_INET;
    addrserv[i].sin_addr.s_addr = INADDR_ANY;
    addrserv[i].sin_port = htons(this->conf.getConfig()[i].listen.second);
    if (bind(servfd[i], (struct sockaddr *)(&addrserv[i]),
             sizeof(addrserv[i])) == -1)
      throw(std::runtime_error("Server: Socket failed to bind"));
    if (listen(servfd[i], MAX_CLENTS) == -1)
      throw(std::runtime_error("Server: Listening failed"));
  }

  std::vector<struct pollfd> fds;
  for (size_t i = 0; i < nserv; i++) {
    struct pollfd a;
    bzero(&a, sizeof(pollfd));
    a.fd = servfd[i];
    a.events = POLLIN;
    fds.push_back(a);
  }
  while (true) {
    poll(&fds[0], fds.size(), -1);
    for (size_t i = 0; i < fds.size(); i++) {
      if (fds[i].revents & POLLIN) {
        if (i < nserv) {
          int client = accept(fds[i].fd, NULL, NULL);
          if (client == -1)
            std::cerr << "accept: A client refused to connect" << std::endl;
          struct pollfd a;
          bzero(&a, sizeof(pollfd));
          a.fd = client;
          a.events = POLLIN;
          fds.push_back(a);
          reqs[client] = "";
        } else {
          if (fds[i].revents & POLLIN) {
            char *data = new char[PAGE];
            int ret = recv(fds[i].fd, data, PAGE, 0);
            data[ret] = 0;
            reqs[fds[i].fd] += std::string(data);
            if(!ret && check_done(reqs[fds[i].fd]))
            {
              
            }

            delete[] data;
          }
        }
      }
      if (i > nserv && fds[i].revents & POLLOUT) {
		
      }
    }
  }
}