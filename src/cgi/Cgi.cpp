/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:22:49 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/27 17:54:18 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include <cctype>
#include <string>
#include <sys/_types/_size_t.h>

Cgi::Cgi() {
  _phpPath = "Src/cgi/cgi-bin/php-cgi";
  _pythonPath = "Src/cgi/cgi-bin/python-cgi";
}
Cgi::~Cgi() {}
Cgi::Cgi(const Cgi &obj) { (void)obj; }
Cgi &Cgi::operator=(const Cgi &obj) {
  (void)obj;
  return *this;
}

Cgi::Cgi(std::map<st_, st_> tmp) {
  this->setEnvVars(tmp);
}

void Cgi::setEnvVars(std::map<st_, st_> map) {
  for (std::map<st_, st_>::iterator it = map.begin(); it != map.end(); it++) {
    st_ tmp = it->first;
    for (size_t i = 0; i < tmp.length(); i++) {
      if (tmp[i] == '-')
        tmp[i] = '_';
      tmp[i] = std::toupper(tmp[i]);
    }
      tmp += "=" + it->second;
      _env.push_back(tmp);
  }
  
  _env.push_back(st_("REQUEST_METHOD="+ req.getMethod_()));
  _env.push_back("REQUEST_URI=" + req.getURI());
  _env.push_back("SCRIPT_NAME=");
  _env.push_back("QUERY_STRING=");
  _env.push_back("DOCUMENT_ROOT=");
  _env.push_back("SERVER_PROTOCOL=HTTP/1.1");
}

void Cgi::execute(st_ uri) { std::cout << uri << std::endl; }