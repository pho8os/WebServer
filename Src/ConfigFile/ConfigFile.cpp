/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaid <absaid@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/29 19:28:42 by absaid            #+#    #+#             */
/*   Updated: 2023/11/08 11:42:56 by absaid           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    int dd = line.find_first_of(";{}");
    while (dd != (int)std::string::npos) {
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

void validateserver(Server &s)
{
  for(int i = 0; i < s.location.size(); i++)
  {
    
  }
}

Config::Config() {
  if (!std::freopen("./Src/ConfigFile/file.config", "r", stdin)) {
    std::cout << "Config file not found" << std::endl;
    std::exit(-1);
  }
  try {
    std::deque<std::string> file = lineget();
    while (file.size())
      this->server.push_back(parseserver(file));
    for(int i = 0; i < this->server.size(); i++)
      validateserver(this->server[i]);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::exit(-1);
  }
}

const std::vector<Server> Config::getConfig() const { return this->server; }
