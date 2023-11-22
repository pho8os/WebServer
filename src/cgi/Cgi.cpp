/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:22:49 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/22 18:08:32 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"

Cgi::Cgi() {
  this->_php_path = "/usr/bin/php";
  this->_php_cgi = "cgi/cgi.php";
}
Cgi::~Cgi() {}
Cgi::Cgi(const Cgi &obj) { (void)obj; }
Cgi &Cgi::operator=(const Cgi &obj) {
  (void)obj;
  return *this;
}
bool Cgi::found_ressource(std::string path) {
  std::cout << path << std::endl;
  if (access(path.c_str(), F_OK) != -1) {
    if (access(path.c_str(), F_OK) == 0)
      return true;
  } else
    std::cerr << "Error : " << errno << std::endl;
  return false;
}

bool Cgi::execute(std::string query) {
  (void)query;
  return true;
}
std::string Cgi::get_ress_path(std::string query) {
  size_t last = query.find_last_of('/');
  if (last != std::string::npos) {
    std::string _name = query.substr(last + 1);
    // std::cout << "Resource name: " << _name << std::endl;
    return _name;
  } else
    std::cerr << "No path found in the URL" << std::endl;
  return NULL;
}
// accepts a url
bool Cgi::makeResponse(const std::string query) {

  if (found_ressource(query)) {
    // part 01 : identify the ressource and
    this->_php_src = get_ress_path(query);
    this->_php_src = query;
    std::string res = getSrcResult();
  } else {
    std::cerr << "Error : Ressource Not Found !!" << std::endl;
    return false;
  }
  return false;
}

std::string Cgi::getSrcResult() {
  std::string res("NULL");
  char *envp[] = {NULL};
  const char *tmpFileName = "temp_out";

  int fd = open(tmpFileName, O_CREAT | O_WRONLY, 0666);
  if (fd == -1) {
    std::cerr << "Error :Failed to create the tmp_fd !!" << std::endl;
    return res;
  }

  int o_out = dup(STDOUT_FILENO);
  if (o_out == -1) {
    std::cerr << "Error :Failed to create the tmp_fd !!" << std::endl;
    close(fd);
    return res;
  }

  if (dup2(fd, STDOUT_FILENO) == -1) {
    std::cerr << "Error :Failed to redirect the stdin to tmp_fd !!"
              << std::endl;
    close(fd);
    return res;
  }

  pid_t a = fork();
  // part 02 : pass the ressource as an argument to the cgi
  if (a == -1) {
    std::cerr << "Fork Failed !!" << std::endl;
    return res;
  }
  if (a == 0) {
    char *args[] = {
      // _envVars["DOCUMENT_ROOT"].c_str(),
      _envVars["HTTP_COOKIE"].c_str(),
      _envVars["HTTP_HOST"].c_str(),
      _envVars["HTTP_REFERER"].c_str(),
      _envVars["HTTP_USER_AGENT"].c_str(),
      _envVars["HTTPS"].c_str(),
      _envVars["PATH"].c_str(),
      _envVars["QUERY_STRING"].c_str(),
      _envVars["REMOTE_ADDR"].c_str(),
      _envVars["REMOTE_HOST"].c_str(),
      _envVars["REMOTE_PORT"].c_str(),
      _envVars["REMOTE_USER"].c_str(),
      _envVars["REQUEST_METHOD"].c_str(),
      _envVars["REQUEST_URI"].c_str(),
      _envVars["SCRIPT_FILENAME"].c_str(),
      _envVars["SCRIPT_NAME"].c_str(),
      _envVars["SERVER_ADMIN"].c_str(),
      _envVars["SERVER_NAME"].c_str(),
      _envVars["SERVER_PORT"].c_str(),
      _envVars["SERVER_SOFTWARE"].c_str()
      
    };

    if (execve(_php_path.c_str(), args, envp) == -1) {
      perror("execve");
      return res;
    }
  } else {
    close(fd);
    dup2(o_out, STDOUT_FILENO);
    close(o_out);
    int stat;
    waitpid(a, &stat, 0);
  }
  return res;
}

void Cgi::setEnvVars(std::map<st_, st_> map) {
  map.insert(map.end(), _envVars.begin());
  (void)map;
}
