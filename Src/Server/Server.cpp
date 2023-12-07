#include "Server.hpp"
#include <cstdlib>
#include <strings.h>
#include <unistd.h>

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
    request req;
    Response resp;
    clients[client].first =  req;// append the request
    clients[client].second = resp; //append the request
  }
  else {
    char * data = new char[PAGE];
    bzero(data, PAGE);
    ssize_t re = recv(fds[index].fd, data, PAGE, 0);
    if (re > 0) {
        clients[fds[index].fd].first.feedMe(st_(data, re));
        if(clients[fds[index].fd].first.getReadStat() == false)
          fds[index].events = POLLOUT;
    } else if (re == 0) {

        close(fds[index].fd);
        fds.erase(fds.begin() + index);
    } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
        perror("recv");
        close(fds[index].fd);
        fds.erase(fds.begin() + index);
    }
    delete [] data;
  }
}
st_ MServer::dummyResp(const st_& content) {
    st_ response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(content.size()) + "\r\n"
        "\r\n" + content;

    return response;
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

      }
      else if(fds[i].revents & POLLOUT)
      {
        // std::cout << "sending" << std::endl;
        st_ resp = dummyResp("<center><h1>TFOOO</h1><center>");
        send(fds[i].fd, resp.c_str(), resp.length(), 0);
        fds[i].events = POLLIN;
      }
    }
  }
}