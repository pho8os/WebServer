
#include "ConfigFile.hpp"
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

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
            (obj == "rederict") * 9 + (obj == "allow") * 11;
    (void)(*f[i])(file, serv);
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
  while (file.size())
    vec.push_back(parseserver(file));
}