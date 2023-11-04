
#include "ConfigFile.hpp"
#include <stdexcept>
#include <string>
#include <vector>

void Server::parslocation(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsindex(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsError_page(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsUp_Path(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsPort(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsRoot(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsHost(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsServer_name(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsMax_Body_size(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsRederict(std::deque<std::string> &file, Server &serv)
{

}
void Server::parsMethods(std::deque<std::string> &file, Server &serv)
{
  char *p = std::strtok((char *)file[0].c_str(), " \t");
  p = std::strtok(NULL, " \t");
  if(!p || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Error in Allow");
  while(p)
  {
      if(std::string(p) == "GET")
        serv.allow[0] = true;
      else if (std::string(p) == "POST")
        serv.allow[1] = true;
      else if (std::string(p) == "DELETE")
        serv.allow[2] = true;
      else
        throw std::runtime_error("Allow : Invalid arg " + std::string(p));
      p = std::strtok(NULL, " \t");
  }
}
void Server::Error(std::deque<std::string> &file, Server &serv)
{
  (void)file;
  (void)serv;
  throw std::runtime_error("undefined key world");
}

void trim(std::string &s) {
  while (s.begin() != s.end() && isspace(*s.begin()))
    s.erase(s.begin());
  while (s.end() != s.begin() && isspace(*s.rbegin()))
    s.pop_back();
}

std::deque<std::string> lineget() {
  std::deque<std::string> file;
  std::string line;
  while (!std::cin.eof()) {
    std::getline(std::cin, line);
    if (line.empty() || line[0] == '#')
      continue;
    (line.find("#") != std::string::npos) &&
        (line = line.substr(0, line.find("#")), 0);
    int dd = line.find_first_of(";{}");
    while (dd != std::string::npos) {
      (line[dd] == '{' || line[dd] == '}') &&
          (line.insert(line.begin() + dd, '\n'), dd++);
      line.insert(line.begin() + dd + 1, '\n');
      int a = line.substr(dd + 1, std::string::npos).find_first_of(";{}");
      if (a == std::string::npos) {
        char *p = std::strtok((char *)line.c_str(), "\n");
        while (p) {
          std::string buff(p);
          trim(buff);
          if (!buff.empty())
            file.push_back(buff + "\n");
          p = std::strtok(NULL, "\n");
        }
        dd = -1;
        break;
      }
      dd += a + 1;
    }
    if (dd == -1)
      continue;
    trim(line);
    file.push_back(line + "\n");
  }
  return file;
}

Server parseserver(std::deque<std::string> &file) {
  Server serv;
  if (file[0] != "server")
    throw std::runtime_error("ghaalat f server");
  file.pop_front();
  if (file[0] != "{")
    throw std::runtime_error("ghaalat f server");
  int check = 1;
  file.pop_front();
  while (check) {
    std::string a[] = {"Error",       "location",      "index",    "error_page",
                       "up_path",     "port",          "root",     "host",
                       "server_name", "max_body_size", "rederict", "allow"};
    (file[0] == "}") && (file.pop_front(), check--);
    std::string obj(std::strtok((char *)std::string(file[0]).c_str(), " \t"));
    int i = (obj == "location") * 1 + (obj == "index") * 2 +
            (obj == "error_page") * 3 + (obj == "up_path") * 4 +
            (obj == "port") * 5 + (obj == "root") * 6 +
            (obj == "host") * 7 + (obj == "server_name") * 8 +
            (obj == "max_body_size") * 9 + (obj == "rederict") * 10 +
            (obj == "allow") * 11;
  }
  return serv;
}

int main() {
  if (!std::freopen("file.config", "r", stdin))
    return (1);
  std::deque<std::string> file = lineget();
  for (int i = 0; i < file.size(); i++)
    std::cout << file[i];
  std::vector<Server> vec;
  while (vec.size())
    vec.push_back(parseserver(file));
}