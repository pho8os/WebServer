
#include "ConfigFile.hpp"
#include <exception>
#include <stdexcept>
#include <sys/_types/_size_t.h>

Methods::Methods() : Get(false), Post(false), Delete(false) {}

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
    size_t dd = line.find_first_of(";{}");
    if (dd != std::string::npos) {
      while (dd != std::string::npos) {
        (line[dd] == '{' || line[dd] == '}') &&
            (line.insert(line.begin() + dd, '\n'), dd++);
        line.insert(line.begin() + dd + 1, '\n');
        size_t a = line.substr(dd + 1, std::string::npos).find_first_of(";{}");
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
    } else {
      trim(line);
      file.push_back(line);
    }
  }
  return file;
}

Server parseserver(std::deque<std::string> &file) {
  Server serv;
  // std::cout << file[0] << std::endl;
  if (file[0] != "server")
    throw std::runtime_error("Server: Error");
  file.pop_front();
  if (file[0] != "{")
    throw std::runtime_error("Server: Server: \"{\" unfound");
  file.pop_front();
  while (file.size()) {
    void (*f[])(std::deque<std::string> &file, Server &Hol) = {
        &Error,          &parslocation,    &parsIndex,
        &parsError_page, &parsUp_Path,     &parsListen,
        &parsRoot,       &parsServer_name, &parsMax_Body_size,
        &parsRederict,   &parsMethods,

    };
    if (file[0] == "}")
      break;
    std::string obj(std::strtok((char *)std::string(file[0]).c_str(), " \t"));

    int i = (obj == "location") * 1 + (obj == "index") * 2 +
            (obj == "error_page") * 3 + (obj == "upload path") * 4 +
            (obj == "listen") * 5 + (obj == "root") * 6 +
            (obj == "server_name") * 7 + (obj == "max_body_size") * 8 +
            (obj == "rederict") * 9 + (obj == "allow") * 10;
    (void)(*f[i])(file, serv);
  }
  if (file[0] != "}")
    throw std::runtime_error("Expecting \"}\" as end point for server");
  file.pop_front();
  return serv;
}
Shared::Shared()
{
  body_size.second = -1;
}
bool Methods::empty()
{
  return((!Get && !Post  && !Delete));
}

void validLocation(Server &serv, Location &loc)
{
  (loc.root.empty()) && (loc.root = serv.root, 0);
  (loc.up_path.empty()) && (loc.up_path = serv.up_path, 0);
  (loc.allow.empty()) && (loc.allow = serv.allow, 0);
  (!loc.index.size()) && (loc.index = serv.index, 0);
  (!loc.error_page.size()) && (loc.error_page = serv.error_page, 0);
  (loc.body_size.second == -1) && (loc.body_size = serv.body_size, 0);
  (loc.redirect.second.empty()) && (loc.redirect = serv.redirect, 0);
}

void validateserver(Server &s) {
  for (size_t i = 0; i < s.location.size(); i++)
    validLocation(s, s.location[i]);
  //if(s.listen.first.empty() || s.root.empty())
  //  throw std::runtime_error("Invalid data server");

}

Config::Config() {
  if (!std::freopen("./Src/ConfigFile/file.config", "r", stdin)) {
    std::cout << "Config file not found" << std::endl;
    std::exit(-1);
  }
  try {
    std::deque<std::string> file = lineget();
    if (!file.size()) {
      std::cout << "Config: error: Empty config" << std::endl;
      std::exit(-1);
    }
    // for(int i = 0; i < (int)file.size(); i++)
    //   std::cout << file[i] << std::endl;
    while (file.size())
      this->server.push_back(parseserver(file));
    for (size_t i = 0; i < this->server.size(); i++)
      validateserver(this->server[i]);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::exit(-1);
  }
}

const std::vector<Server> Config::getConfig() const { return this->server; }
