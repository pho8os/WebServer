#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <utility>
template <typename T>
 void parsIndex(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Index: error");
  while (p) {
    Hol.index.push_back(std::string(p));
    p = std::strtok(NULL, " \t");
  }
  file.pop_front();
}

template <typename T>
 void parsCgi(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  char *q = std::strtok(NULL, " ;\t");
  if (!p || !q || std::strtok(NULL, " \t") || file[0][file.size() - 1] != ';')
    throw std::runtime_error("Cgi: error");
  std::string key(p);
  std::string value(q);
  if (key != "py" || key != "php")
    throw std::runtime_error("Cgi: error: " + key + ": Invalid key");
  Hol.cgi[key] = value;
  file.pop_back();
}

template <typename T>
void parsAutoindex(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Auto index: error");
  std::string s(p);
  if (s != "ON" && s != "OFF")
    throw std::runtime_error("Auto index: error");
  Hol.autoindex = (s == "ON") * true + (s == "OFF") * false;
  file.pop_front();
}

template <typename T>

void parsError_page(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  char *q = std::strtok(NULL, " ;\t");
  if (!p || !q || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Error pages: error");
  file[0].pop_back();
  while (p && q) {
    
    for(int i = 0; p[i]; i++)
      if(!std::isdigit(p[i]))
        throw std::runtime_error("Error pages: Invalid value");
    Hol.error_page[std::atoi(p)] = std::string(q);
    p = std::strtok(NULL, " \t");
    q = std::strtok(NULL, " ;\t");
  }
  if(p && !q)
    throw std::runtime_error("Error pages : value missing");
  file.pop_front();
}

template <typename T>
 void parsUp_Path(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || std::strtok(NULL, " ;\t") || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Upload path: error");
  file[0].pop_back();
  Hol.up_path = std::string(p);
  file.pop_front();
}

template <typename T>
void parsListen(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || std::strtok(NULL, " ;\t") || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Listen: error");
  // Hol.port.push_back(std::atoi(p));
  std::string obj(p);
  size_t f = obj.find(":");
  std::string first("localhost");
  int second = 0;
  (f != std::string::npos && f == obj.rfind(":") && f) &&
      (first = obj.substr(0, f), obj = obj.substr(f + 1), 0);
  for (size_t i = 0; i < obj.size(); i++)
    if (!std::isdigit(obj[i]))
      throw std::runtime_error("Listen: error");
  second = std::atoi(obj.c_str());
  Hol.listen = std::make_pair(first, second);
  file.pop_front();
}

template <typename T>
 void parsRoot(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || std::strtok(NULL, " ;\t") || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("root: error");
  Hol.root = std::string(p);
  file.pop_front();
}

template <typename T>
 void parsHost(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || std::strtok(NULL, " ;\t") || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("host: error");
  file[0].pop_back();
  Hol.host = std::string(p);
  file.pop_front();
}

template <typename T>

void parsServer_name(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || std::strtok(NULL, " ;\t") || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("server name: error");
  file[0].pop_back();
  Hol.server_name = std::string(p);
  file.pop_front();
}

template <typename T>
void parsMax_Body_size(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  if (!p || std::strtok(NULL, " ;\t") || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Max_Body_Size: error");
  char specifier = 'B';
  int i = -1;
  while(p[++i]) 
    if(!std::isdigit(p[i]))
      break;
  (p[i] && (p[i] == 'M' || p[i] == 'G'|| p[i] == 'K' || p[i] == 'B')) && (specifier = p[i], i++);
  if(p[i])
    throw std::runtime_error("Max_Body_Size: error");
  Hol.body_size = std::make_pair(std::stoull(p), specifier);
  file.pop_front();
}

template <typename T>
 void parsRederict(std::deque<std::string> &file, T &Hol) {
  char *p = std::strtok(NULL, " ;\t");
  char *q = std::strtok(NULL, " ;\t");
  if (!p || !q || std::strtok(NULL, " ;\t") || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Rederict: error");
  for(size_t i = 0; q[i]; i++)
    if(!std::isdigit(q[i]))
      throw std::runtime_error("Rederict: error");
  Hol.redirect.first = std::atoi(q);
  Hol.redirect.second = std::string(p);
  file.pop_front();
}

template <typename T>
 void parsMethods(std::deque<std::string> &file, T &Hol) {

  char *p = std::strtok(NULL, " ;\t");
  if (!p || file[0][file[0].size() - 1] != ';')
    throw std::runtime_error("Error in Allow");
  int i = -1;
  while (p && ++i < 3) {
    if (std::string(p) == "GET")
      Hol.allow.Get = true;
    else if (std::string(p) == "POST")
      Hol.allow.Post = true;
    else if (std::string(p) == "DELETE")
      Hol.allow.Delete = true;
    else
      throw std::runtime_error("Allow : Invalid arg " + std::string(p));
    p = std::strtok(NULL, " ;\t");
  }
  if (p)
    throw std::runtime_error("Allow : Too much args");
  file.pop_front();
}
template <typename T>
 void Error(std::deque<std::string> &file, T &Hol) {
  (void)file;
  (void)Hol;
  throw std::runtime_error("undefined key world " + file[0]);
}
template <typename T>
 void parslocation(std::deque<std::string> &file, T &Hol) {
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
    std::string obj(std::strtok((char *)(std::string(file[0])).c_str(), " \t"));
    int i = (obj == "index") * 1 + (obj == "error_page") * 2 +
            (obj == "up_path") * 3 + (obj == "root") * 4 +
            (obj == "max_body_size") * 5 + (obj == "rederict") * 6 +
            (obj == "allow") * 7 + (obj == "autoindex") * 8 +
            (obj == "cgi") * 9;
    (void)(*f[i])(file, loc);
  }

  if (file[0] != "}")
    throw std::runtime_error("Expecting \"}\" as end point for location");
  file.pop_front();
  Hol.location.push_back(loc);
}