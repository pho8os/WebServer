
#include "ConfigFile.hpp"
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

Methods::Methods() : Get(false), Post(false), Delete(false) {}

template <typename T> void parsIndex(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " \t");
  if (!p || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Index: error");
  file[0].pop_back();
  while (p) {
    Hol.index.push_back(std::string(p));
    p = std::strtok(NULL, " \t");
  }
  file.pop_back();
}

template <typename T> void parsCgi(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " \t");
  char *q = std::strtok(NULL, " \t");
  if (!p || !q || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Cgi: error");
  file[0].pop_back();
  std::string key(p);
  std::string value(q);
  if(key != "py" || key != "php")
    throw std::runtime_error("Cgi: error: " + key + ": Invalid key");
  Hol.cgi[key] = value;
  file.pop_back();
}

template <typename T>
void parsAutoindex(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " \t");
  if (!p || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Auto index: error");
  file[0].pop_back();
  std::string s(p);
  if (s != "ON" && s != "OFF")
    throw std::runtime_error("Auto index: error");
  Hol.autoindex = (s == "ON") * true + (s == "OFF") * false;
  file.pop_front();
}

template <typename T>
void parsError_page(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " \t");
  if (!p || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Auto index: error");
  file[0].pop_back();
  while (p) {
    // Hol.error_page.push_back(std::string(p));
    p = std::strtok(NULL, " \t");
  }
  file.pop_front();
}

template <typename T> void 
parsUp_Path(std::deque<std::string> &file, T &Hol) 
{
  char *p = std::strtok(NULL, " \t");
  if (!p || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Upload path: error");
  file[0].pop_back();
  Hol.up_path = std::string(p);
  file.pop_front();
}

template <typename T> void 
parsPort(std::deque<std::string> &file, T &Hol) 
{
  char *p = std::strtok(NULL, " \t");
  if (!p || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Listen: error");
  file[0].pop_back();
  while(p)
  {
    for(int i = 0; p[i]; i++)
      if(!std::isdigit(p[i]))
        throw std::runtime_error("Listen:" + std::string(p) + ": error");
    Hol.port.push_back(std::atoi(p));      
    p = std::strtok(NULL, " \t");
  }
  file.pop_front();
}

template <typename T> void 
parsRoot(std::deque<std::string> &file, T &Hol) 
{
  char *p = std::strtok(NULL, " \t");
  if (!p || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("root: error");
  file[0].pop_back();
  Hol.root = std::string(p);
  file.pop_front();
}

template <typename T> void 
parsHost(std::deque<std::string> &file, T &Hol) 
{
  char *p = std::strtok(NULL, " \t");
  if (!p || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("host: error");
  file[0].pop_back();
  Hol.host = std::string(p);
  file.pop_front();
}

template <typename T>
void parsServer_name(std::deque<std::string> &file, T &Hol) 
{
  char *p = std::strtok(NULL, " \t");
  if (!p || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("server name: error");
  file[0].pop_back();
  Hol.server_name = std::string(p);
  file.pop_front();
}

template <typename T>
void parsMax_Body_size(std::deque<std::string> &file, T &Hol)
{
  char *p = std::strtok(NULL, " \t");
  if (!p || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Upload path: error");
  file[0].pop_back();
  for(int i = 0; p[i]; i++)
      if(!std::isdigit(p[i]))
        throw std::runtime_error("Max_body_size:" + std::string(p) + ": error");
  Hol.body_size = std::stoull(p);
  file.pop_front();
}

template <typename T>
void parsRederict(std::deque<std::string> &file, T &Hol) 
{
  char *p = std::strtok(NULL, " \t");
  if (!p || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Rederict: error");
  file[0].pop_back();
  Hol.redirect = std::string(p);
  file.pop_front();
}

template <typename T> void parsMethods(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok((char *)file[0].c_str(), " \t");
  p = std::strtok(NULL, " \t");
  if (!p || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Error in Allow");
  while (p) {
    if (std::string(p) == "GET")
      Hol.allow.Get = true;
    else if (std::string(p) == "POST")
      Hol.allow.Post = true;
    else if (std::string(p) == "DELETE")
      Hol.allow.Delete = true;
    else
      throw std::runtime_error("Allow : Invalid arg " + std::string(p));
    p = std::strtok(NULL, " \t");
  }
}
template <typename T> void Error(std::deque<std::string> &file, T &Hol) {
  (void)file;
  (void)Hol;
  throw std::runtime_error("undefined key world");
}
template <typename T> void parslocation(std::deque<std::string> &file, T &Hol) {
  Location loc;
  char *p = std::strtok(NULL, " \t");
  if (p)
    loc.prefix = std::string(p);
  if (std::strtok(NULL, " \t") || !p)
    throw std::runtime_error("location: prefix: error");
  file.pop_front();
  if (file[0] != "{")
    throw std::runtime_error("Location Error");
  file.pop_front();
  while (file.size()) {
    if (file[0] == "}")
      break;
    void (*f[])(std::deque<std::string> &file, Location &Hol) = {
        &Error,         &parsIndex,         &parsError_page, &parsUp_Path,
        &parsRoot,      &parsMax_Body_size, &parsRederict,   &parsMethods,
        &parsAutoindex, &parsCgi,
    };
    std::string obj(std::strtok((char *)file[0].c_str(), " \t"));
    int i = (obj == "index") * 1 + (obj == "error_page") * 2 +
            (obj == "up_path") * 3 + (obj == "root") * 4 +
            (obj == "max_body_size") * 5 + (obj == "rederict") * 6 +
            (obj == "allow") * 7 + (obj == "autoindex") * 8 +
            (obj == "cgi") * 9;
  }
  if (file[0] != "}")
    throw std::runtime_error("Expecting \"}\" as end point for location");
  file.pop_front();
  Hol.location.push_back(loc);
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
          (!buff.empty()) && (file.push_back(buff), 0);
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
    file.push_back(line);
  }
  return file;
}

Server parseserver(std::deque<std::string> &file) {
  Server serv;
  if (file[0] != "server")
    throw std::runtime_error("Server: Error");
  file.pop_front();
  if (file[0] != "{")
    throw std::runtime_error("Server: Server");
  file.pop_front();
  while (file.size()) {
    void (*f[])(std::deque<std::string> &file, Server &Hol) = {
        &Error,           &parslocation,      &parsIndex,    &parsError_page,
        &parsUp_Path,     &parsPort,          &parsRoot,     &parsHost,
        &parsServer_name, &parsMax_Body_size, &parsRederict, &parsMethods,

    };
    if (file[0] == "}")
      break;
    std::string obj(std::strtok((char *)file[0].c_str(), " \t"));
    int i = (obj == "location") * 1 + (obj == "index") * 2 +
            (obj == "error_page") * 3 + (obj == "up_path") * 4 +
            (obj == "port") * 5 + (obj == "root") * 6 + (obj == "host") * 7 +
            (obj == "server_name") * 8 + (obj == "max_body_size") * 9 +
            (obj == "rederict") * 10 + (obj == "allow") * 11;
  }
  if (file[0] != "}")
    throw std::runtime_error("Expecting \"}\" as end point for server");
  file.pop_front();
  return serv;
}

int main() {
  if (!std::freopen("file.config", "r", stdin))
    return (1);
  std::deque<std::string> file = lineget();
  for (int i = 0; i < file.size(); i++)
    std::cout << file[i] << std::endl;
  std::vector<Server> vec;
  //while (file.size())
  //  vec.push_back(parseserver(file));
}