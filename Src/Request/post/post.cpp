/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 12:49:02 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/25 16:27:23 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "post.hpp"
#include <exception>
#include <sys/_types/_size_t.h>
#include <utility>
#include <vector>

post::post() {}

post::~post() {}

std::vector<st_> post::ft_split(const st_ req, const st_ del) {
  std::vector<std::string> res;
  size_t begin = 0;
  size_t end = req.length();

  while (begin < end) {
    size_t pos = req.find(del, begin);

    if (pos != std::string::npos) {
      std::string t = req.substr(begin, pos - begin);
      res.push_back(t);
      begin = pos + del.length();
    } else {
      std::string t = req.substr(begin, end - begin);
      res.push_back(t);
      begin = end;
    }
  }
  return res;
}

st_ post::fileToStr(const st_ path) {
  std::ifstream file(path.c_str());
  if (file.is_open()) {
    st_ ctn;
    char tmp;
    while (file.get(tmp))
      ctn += tmp;
    file.close();
    return ctn;
  } else
    throw std::runtime_error("Could Not Open File !!");
  return NULL;
}

void post::detectBoundry() {
  if (isValidReq()) {

    Map mp = req.getVector();
    st_ line = mp["Content-Type"];
    this->_boundary = line.substr(line.find("boundary=") + 9, line.length());
    this->_endBoundary = "--" + _boundary + "--";
  }
}

bool post::isValidReq() { return true; }

void post::detectFields() {
  st_ body = this->req.getBody();
  _fields = ft_split(body, "\r\n--" + _boundary + "\r\n");
}

void post::detectBinaryFiles() {
  for (size_t i = 0; i < _fields.size(); i++) {
    if (_fields[i].find("Content-Type:") != st_::npos) {
      _binFiles.push_back(_fields[i]);
      _fields.erase(_fields.begin() + i);
    }
  }
  size_t pos = _fields[_fields.size() - 1].find(_endBoundary);
  if (_binFiles.size() && pos != st_::npos)
    _binFiles[_binFiles.size() - 1] = _binFiles[_binFiles.size() - 1] =
        ft_split(_binFiles[_binFiles.size() - 1], _endBoundary)[0];
}

void post::makeResponse(int code) { std::cout << code << std::endl; }

// extract fileName && fileBody
// void post::parseFiles() {
//   for (size_t i = 0; i < _binFiles.size(); i++) {
//     size_t beg = _binFiles[i].find("filename");
//     if (beg != st_::npos) {
//       st_ header = _binFiles[i].substr(0, _binFiles[i].find("\r\n\r\n"));
//       st_ body = _binFiles[i].substr(_binFiles[i].find("\r\n\r\n") + 4);
//       _binFiles[i] = body;
//       size_t a = header.find("filename=\"");
//       size_t b = header.find("\"\r\n");
//       header = header.substr(a + 10, b - (a + 10));
//       _binFileNames.push_back(header);
//     }
//   }
// }

void post::makeFiles() {
  for (size_t i = 0; i < _binFiles.size(); i++) {
    std::ofstream ofile(std::string(_upPath + _binFileNames[i].c_str()));

    if (!ofile.is_open() || ofile.bad())
      makeResponse(503);

    ofile << _binFiles[i];
    if (ofile.bad()) {
      ofile.close();
      makeResponse(503);
    }
    ofile.close();
  }
}

void post::runPost(st_ path) {
  std::string st = fileToStr(path.c_str());
  try {
    this->req = request(st);
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  std::map<st_, st_> tmp_map = req.getVector();
  if (tmp_map["Content-Type"].find("multipart/form-data") != st_::npos) {
    detectBoundry();
    detectFields(); // first field still with its boundary
    detectBinaryFiles();
    detectDataFields();
    // parseFiles();
    makeFiles();
  } else if (tmp_map["Content-Type"].find(
                 "application/x-www-form-urlencoded") != st_::npos) {
    detectUrlFields();
  } else
    throw 403;
}

void post::detectDataFields() {
  for (size_t i = 0; i < _fields.size(); i++) {
    st_ key = ft_split(_fields[i], "name=\"")[1];
    key = key.substr(0, key.find('"'));
    st_ val = _fields[i].substr(_fields[i].find_last_of("\r\n") + 1);
    _dataFields.push_back(std::make_pair(key, val));
  }
}

void post::detectUrlFields() {
  st_ body = this->req.getBody();
  st_ str = ft_split(body, "\r\n")[0];
  std::vector<st_> tmp = ft_split(str, "&");
  for (size_t i = 0; i < tmp.size(); i++) {
    std::vector<st_> ap = ft_split(tmp[i], "=");
    _urlFields.push_back(std::make_pair(ap[0], ap[1]));
  }
}

void post::setUpPath(const st_ &str)
{
  this->_upPath = str;
}
