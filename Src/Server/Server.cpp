#include "Server.hpp"
#include <strings.h>
#include <sys/poll.h>
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
  st_ content = "<h1><center>Welcome</center></h1>";
  nserv = Config::getConfig().size();
      resp << "HTTP/1.1 200 OK\r\n";
      resp << "Content-Type: text/html\r\n";
      resp << "Content-Length: " << content.length() << "\r\n";
      resp << "\r\n";
      resp << content;
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
      fcntl(sock, F_GETFL);
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

void MServer::handlePost(st_ t_str, size_t size)
{
      if (A.chunked == true)
      {
          // std::cout << "[LOG] => imta kanlanceeew parsepages\n";
        tmp.parsepage(t_str, this->A);
      }
      else
      {
          // std::cout << "[LOG] => imta kanlanceeew parse2pages\n";
          tmp.parse2pages(t_str, this->A);
          std::cout << "-----><------" << A.reading << std::endl;
          // std::cout << "[LOG] => khrejna mn page2pages\n";
          // if (!A.reading)
      }
}

void MServer::acceptClient(const size_t &index)
{
    int client = accept(fds[index].fd, NULL, NULL);
    std::cout << "\n********-*********-**********-********-*********\n";
    std::cout << "[event] -----> New client Connected [FD] : " << client <<std::endl;
    if (client == -1)
      std::cerr << "accept: A client refused to connect" << std::endl;

    tmp_req[client].first = tmp;
    tmp_req[client].second = tmp_resp;

    struct pollfd a;
    bzero(&a, sizeof(pollfd));
    a.fd = client;
    a.events = POLLIN;

    fds.push_back(a);
    reqs[index] = "";
}
void MServer::handleClient(const size_t &index)
{
    A.upPath = "/goinfre/zmakhkha/up/";
    char *data = new char[PAGE];
    bzero(data, PAGE);
    size_t size = recv(fds[index].fd, data, PAGE, 0);
    
    // handle the first time conection
    if (!reqs[index].length())
    {
      reqs[index]+= std::string(data, size);
      tmp.parseMe(st_(data, size));
      A.boundary = "--" + tmp.getBoundary();
      std::map<st_, st_> tmap = tmp.getVector();
      this->A.chunked = false;
    }
    if (tmp.getMethod_() == "POST")
      handlePost(st_(data), size);
    else {
      A.reading = 0;
    }

    // Disconnect the client
    if (!A.reading)
    {
      tmp_resp.RetResponse( tmp);
      send(fds[index].fd, tmp_resp.getRet().c_str(), tmp_resp.getRet().length(), 0);
      // std::cout << tmp_resp.getRet() << std::endl;
      close(fds[index].fd);
          std::cout << "[event] -----> OLD client DISConnected [FD] : " << fds[index].fd <<std::endl;
      fds.erase(fds.begin() + index );
    }
    delete []data;

}

void MServer::receiving(const size_t &index)
{
  if(index < servfd.size())
    acceptClient(index);
  else 
    handleClient(index);
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
          std::cout << "-------> hahua \n";
        close(fds[i].fd);
        fds.erase(fds.begin() + i );
      }
      else if(fds[i].revents & POLLIN)
      {
        this->receiving(i);
        break;
      }
      else if(fds[i].revents & POLLOUT)
      {
        // std::cout << "-->> " << tmp_req[i].getConnection() << " | " << A.reading << std::endl;
        if(tmp_req[i].first.getConnection() == 0 && A.reading == 0)
        {
          // fds[i].events = POLLHUP;
          // send(fds[i].fd, resp.c_str(), resp.length(), -1);
          // std::cout << resp << std::endl;
          // close(fds[i].fd);
          // fds.erase(fds.begin() + i );
          // std::cout << "clooosito \n";
          // exit(0);
        }

        // fds[i].events = POLLIN;
        // resp = makeResponse();
        // send(fds[i].fd, resp.c_str(), resp.length(), -1);
      }
    }
  }
}
