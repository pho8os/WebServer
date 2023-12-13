#include "Server.hpp"
#include <cstddef>
#include <cstdlib>
#include <strings.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <unistd.h>

std::string file_gen() {
  static size_t i;
  return (std::string("/tmp/file_") + std::to_string(i++));
}

bool MServer::port_exist(size_t &index) const {
  for (size_t i = 0; i < index; i++)
    if (servers[i].listen == servers[index].listen) {

      std::cout << servers[index].listen.first;
      return true;
    }
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
      sockaddr_in addrserv[nserv];
      addrserv[i].sin_family = AF_INET;
      addrserv[i].sin_addr.s_addr = INADDR_ANY;
      addrserv[i].sin_port = htons(std::atoi(servers[i].listen.second.c_str()));
      if (bind(sock, (struct sockaddr *)(&addrserv[i]), sizeof(addrserv[i])) ==
          -1)
        throw(std::runtime_error("Server: Socket failed to bind"));
      if (listen(sock, SOMAXCONN) == -1)
        throw(std::runtime_error("Server: Listening failed"));
      struct pollfd a;
      bzero(&a, sizeof(pollfd));
      a.fd = sock;
      a.events = POLLIN;
      fds.push_back(a);
      servfd.push_back(sock);
      std::cout << "Server Listening on port : " << servers[i].listen.second.c_str() << std::endl;
    }
  }
  this->run();
}

void MServer::acceptClient(const size_t &index) {
  int client = accept(fds[index].fd, NULL, NULL);
  if (client == -1)
    std::cerr << "accept: A client refused to connect" << std::endl;
  std::cout << getTime() << "Client with FD [" << client << "] connected !"
            << std::endl;
  struct pollfd a;
  bzero(&a, sizeof(pollfd));
  a.fd = client;
  a.events = POLLIN;
  fds.push_back(a);
  request tmpReq;
  Response tmpResp;
  clients[client].first = tmpReq;
  clients[client].second = tmpResp;
}

void MServer::sendResp(const size_t &index) {
  clients[fds[index].fd].second.RetResponse(clients[fds[index].fd].first);
  send(fds[index].fd, clients[fds[index].fd].second.getRet().c_str(),
       clients[fds[index].fd].second.getRet().length(), 0);
  std::cout << getTime() << "Client with FD [" << fds[index].fd
            << "] disconnected !" << std::endl;
  clients.erase(fds[index].fd);
  close(fds[index].fd);
  fds.erase(fds.begin() + index);
}

void MServer::handleClient(const size_t &index) {
  if (index < servfd.size()) {
    acceptClient(index);
  } else {
    char *data = new char[PAGE];
    // bzero(data, PAGE);
    ssize_t re = recv(fds[index].fd, data, PAGE, 0);
    if (re > 0) {
      clients[fds[index].fd].first.feedMe(st_(data, re));
      if (clients[fds[index].fd].first.getReadStat() == false) {
        sendResp(index);
      }
    } else if (!clients[fds[index].fd].first.getReadStat() || !re) {
      sendResp(index);
    } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
      perror("recv");
      close(fds[index].fd);
      fds.erase(fds.begin() + index);
    }
    delete[] data;
  }
}

void MServer::receiving(const size_t &index) {
  if (index < servfd.size()) {
    acceptClient(index);
  } else {
    handleClient(index);
  }
}
st_ MServer::dummyResp(const st_ &content) {
  st_ response = "HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/plain\r\n"
                 "Content-Length: " +
                 std::to_string(content.size()) +
                 "\r\n"
                 "\r\n" +
                 content;

  return response;
}

// void MServer::sending(const size_t &index)
// {
//   if(!this->clients[index].second.headersent)

    
// }

void MServer::run() {
  std::cout << "Reading [" << PAGE << "] \n";
  while (true) {
    int timeout = (fds.size() == nserv) * -1 + (fds.size() > nserv) * 6000;
    poll(&fds[0], fds.size(), timeout);
    for (size_t i = 0; i < fds.size(); i++) {
      if (fds[i].revents & POLLHUP) {
        std::cout << getTime() << "Client with FD [" << fds[i].fd
                  << "] disconnected normally!" << std::endl;
        // close(fds[i].fd);
        fds.erase(fds.begin() + i);
      } else if (fds[i].revents & POLLIN) {
        this->receiving(i);
      } else if (fds[i].revents & POLLOUT) {
        // this->sending(i);
        // std::cout << "sending" << std::endl;
      }
    }
  }
}

st_ MServer::getTime() {
  std::time_t currentTime = std::time(nullptr);

  std::tm *localTime = std::localtime(&currentTime);

  std::ostringstream oss;
  oss << "[" << std::setfill('0') << std::setw(2) << localTime->tm_hour << " : "
      << std::setfill('0') << std::setw(2) << localTime->tm_min << " : "
      << std::setfill('0') << std::setw(2) << localTime->tm_sec << "] ";
  return oss.str();
}