
#include "ConfigFile.hpp"
#include <deque>
#include <exception>
#include <stdexcept>
#include <sys/_types/_size_t.h>
#include <vector>

Methods::Methods() : Get(false), Post(false), Delete(false) {}

void trim(std::string &s) {
  while (s.begin() != s.end() && isspace(*s.begin()))
    s.erase(s.begin());
  while (s.end() != s.begin() && isspace(*s.rbegin()))
    s.pop_back();
}

void push_multiline(std::deque<std::string> &file, std::string &line,
                    size_t &dd) {
  while (dd != std::string::npos) {
    (line[dd] == '{' || line[dd] == '}') &&
        (line.insert(line.begin() + dd, '\n'), dd++);
    line.insert(line.begin() + dd + 1, '\n');
    size_t a = line.substr(dd + 1, std::string::npos).find_first_of(";{}");
    dd = (a == std::string::npos) * a + (a != std::string::npos) * (a + dd + 1);
  }
  char *p = std::strtok((char *)line.c_str(), "\n");
  while (p) {
    std::string buff(p);
    trim(buff);
    (!buff.empty()) && (file.push_back(buff), 0);
    p = std::strtok(NULL, "\n");
  }
}

void valid_line(std::deque<std::string> &file, std::string &line) {
  trim(line);
  (line.find("#") != std::string::npos) &&
      (line = line.substr(0, line.find("#")), 0);
  if (line.empty())
    return;
  size_t dd = line.find_first_of(";{}");
  if (dd != std::string::npos)
    return (push_multiline(file, line, dd), (void)0);
  trim(line);
  (!line.empty()) && (file.push_back(line), 0);
}

std::deque<std::string> getfile(const std::string &path)
{
  if (!std::freopen(path.c_str(), "r", stdin)) {
    std::cout << "Config file not found" << std::endl;
    std::exit(-1);
  }
  std::deque<std::string> file;
  std::string line;
  while(!std::cin.eof())
  {
    std::getline(std::cin , line);
    valid_line(file, line);
  }
  return file;
}

Server parseserver(std::deque<std::string> &file) {
  Server serv;
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
        &parsRedirect,   &parsMethods,
    };
    if (file[0] == "}")
      break;
    std::string str = file[0];
    char *tok = std::strtok((char *)str.c_str(), " ;\t");
    if (!tok)
      throw std::runtime_error("error: " + file[0]);
    std::string obj(tok);
    int i = (obj == "location") * 1 + (obj == "index") * 2 +
            (obj == "error_page") * 3 + (obj == "upload path") * 4 +
            (obj == "listen") * 5 + (obj == "root") * 6 +
            (obj == "server_name") * 7 + (obj == "max_body_size") * 8 +
            (obj == "redirect") * 9 + (obj == "allow") * 10;
    (void)(*f[i])(file, serv);
  }
  if (file[0] != "}")
    throw std::runtime_error("Expecting \"}\" as end point for server");
  file.pop_front();
  return serv;
}
Shared::Shared() { body_size.second = -1; }
bool Methods::empty() { return ((!Get && !Post && !Delete)); }

void validLocation(Server &serv, Location &loc) {
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
}

std::vector<Server> Config::server;

std::vector<Server> parseconf(const std::string &path) {
  std::vector<Server> server;
  try {
    std::deque<std::string> file = getfile(path);
    if (!file.size()) {
      std::cout << "Config: error: Empty config" << std::endl;
      std::exit(-1);
    }
    while (file.size())
      server.push_back(parseserver(file));
    for (size_t i = 0; i < server.size(); i++)
      validateserver(server[i]);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::exit(-1);
  }
  std::cin.clear();
  return (server);
}

void Config::setConfig(std::vector<Server> serv)
{
  server = serv;
}

const std::vector<Server> &Config::getConfig() const { return this->server; }

void Config::print_config() const
{
  for(size_t i = 0; i < this->server.size(); i++)
  {
     std::vector<Location> loc = this->server[i].location;
    std::cout <<  "server" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "\tlisten\t" << server[i].listen.first << ":" << server[i].listen.second << ";" << std::endl;
      std::cout << "\tindex\t";
    for (size_t j = 0; j < server[i].index.size(); j++)
    {
        std::cout << server[i].index[j] << "\t";
    }
    std::cout << std::endl;
    for(size_t j = 0; j < loc.size(); j++)
    {
      std::cout << "\tlocation\t" <<  loc[j].prefix << std::endl;
      std::cout << "\t{" << std::endl;
      std::cout << "\t\troot\t" <<  loc[j].root << ";" << std::endl;
      std::cout << "\t\tAutoindex\t" << ((loc[j].autoindex) ? "ON" : "OFF") << ";" << std::endl;
      std::cout << "\t}" << std::endl;
    }
      std::cout << "}" << std::endl;
  }
}