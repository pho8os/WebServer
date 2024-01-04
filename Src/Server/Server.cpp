#include "Server.hpp"
#include <cstddef>
#include <strings.h>
#include <sys/_types/_off_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

std::string file_gen() {
  static size_t i;
  return (std::string("/tmp/file_") + std::to_string(i++));
}

bool MServer::port_exist(size_t &index) const {
  for (size_t i = 0; i < index; i++)
    if (servers[i].listen.second == servers[index].listen.second)
      return true;
  return false;
}

MServer::MServer() : servers(Config::getConfig()) {
  nserv = Config::getConfig().size();
}

MServer::~MServer() {}

void MServer::Serving() {
  for (size_t i = 0; i < nserv; i++) {
    if (!port_exist(i)) {
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      if (sock < 0)
        throw std::runtime_error("Server: creating socket failed");
      int option = 1;
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option,
                 sizeof(option));
      if (sock < 0)
        throw std::runtime_error("Server: creating socket failed");
      sockaddr_in addrserv;
      addrserv.sin_family = AF_INET;
      addrserv.sin_addr.s_addr = INADDR_ANY;
      addrserv.sin_port = htons(std::atoi(servers[i].listen.second.c_str()));
      if (bind(sock, (struct sockaddr *)(&addrserv), sizeof(addrserv)) == -1)
        throw(std::runtime_error("Server: Socket failed to bind"));
      if (listen(sock, SOMAXCONN) == -1)
        throw(std::runtime_error("Server: Listening failed"));
      fcntl(sock, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
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

void MServer::logerror(const size_t &index, std::string cmd) {
  perror(cmd.c_str());
  std::map<int , request>::iterator itreq;
  std::map<int , Response>::iterator itres;

  itreq = Reqs.find(fds[index].fd);
  if(itreq != Reqs.end())
    Reqs.erase(Reqs.find(fds[index].fd));
  itres = Resp.find(fds[index].fd);
  if(itres != Resp.end())
    Resp.erase(Resp.find(fds[index].fd));
  close(fds[index].fd);
  fds.erase(fds.begin() + index);
}

bool MServer::sending(const size_t &index) {
  off_t len = PAGE;
  Response &obj = Resp[fds[index].fd];
  if (obj.sending) {
    if (!obj.headersent) {
      std::string &str = obj.getRet();
      if (str.size() >= PAGE) {
        if (send(fds[index].fd, str.c_str(), PAGE, 0) == -1)
          return (logerror(index, "send"), false);
        str = str.erase(0, PAGE);
        return true;
      } else {
        ssize_t re = send(fds[index].fd, str.c_str(), str.size(), 0);
        if (re == -1)
          return (logerror(index, "send"), false);
        obj.headersent = true;
        if (obj.getFd() == -1) {
          obj.sending = false;
          return true;
        }
        len -= re;
      }
    }
    if (obj.getFd() != -1) {
      char *data = new char[len];
      ssize_t re = read(obj.getFd(), data, len);
      if (re == -1)
        return (delete[] data, logerror(index, "read"), false);
      if (!re)
        return (delete[] data, obj.sending = false, (void)0, true);
      if (send(fds[index].fd, data, re, 0) == -1)
        return (delete[] data, logerror(index, "send"), false);
      delete[] data;
    }
  }
  if (!obj.sending) {
    request objj = Reqs[fds[index].fd];
    bool var = objj.getConnection();
    Reqs[fds[index].fd].clear_Obj();
    Resp[fds[index].fd].clear_Obj();
    Reqs.erase(Reqs.find(fds[index].fd));
    Resp.erase(Resp.find(fds[index].fd));
    if (!var) {
      close(fds[index].fd);
      fds.erase(fds.begin() + index);
    }
  }
  return true;
}

bool MServer::receiving(const size_t &index) {
  if (index < servfd.size()) {
    int client = accept(servfd[index], NULL, NULL);
    if (client == -1)
      return (std::cerr << "accept: A client refused to connect" << std::endl,
              false);
    fcntl(client, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    struct pollfd a;
    bzero(&a, sizeof(pollfd));
    a.fd = client;
    a.events = POLLIN;
    fds.push_back(a);
  } else {
    if (Reqs.find(fds[index].fd) == (Reqs.end()))
      Reqs[fds[index].fd] = request();
    request &obj = Reqs[fds[index].fd];
    char *data = new char[PAGE];
    bzero(data, PAGE);
    ssize_t re = recv(fds[index].fd, data, PAGE, 0);
    if (re == -1)
      return (delete[] data, logerror(index, "recv"), false);
    if (obj.reading && re > 0)
      obj.feedMe(std::string(data, re));
    if (!obj.reading) {
      fds[index].events = POLLOUT;
      Response resp;
      Resp.insert(std::make_pair(fds[index].fd, resp.RetResponse(obj)));
    }
    delete[] data;
  }
  return true;
}

void MServer::run() {
  while (true) {
    int timeout = (fds.size() == nserv) * -1 + (fds.size() > nserv) * 6000;
    int x = poll(&fds[0], fds.size(), timeout);
    if (x < 0)
      continue;
    if (!x) {
      for (size_t i = servfd.size(); i < fds.size(); i++) {
        close(fds[i].fd);
        fds.erase(fds.begin() + i);
      }
    }
    for (size_t i = 0; i < fds.size(); i++) {
      if (fds[i].revents & POLLIN)
      {
        if (!this->receiving(i))
          break;
      }
      else if (fds[i].revents & POLLOUT)
      {
        if (this->sending(i))
          break;
      }
      else if (fds[i].revents & POLLHUP) {
        Reqs.erase(Reqs.find(fds[i].fd));
        Resp.erase(Resp.find(fds[i].fd));
        close(fds[i].fd);
        fds.erase(fds.begin() + i);
      }
    }
  }
}
