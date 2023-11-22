/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 12:49:02 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/22 10:31:51 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "post.hpp"
#include <sys/_types/_size_t.h>
#include <utility>
#include <vector>

post::post() { _upPath = "/Users/zmakhkha/Desktop/WebServer/upload/"; }

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
  detectFields();
  for (size_t i = 0; i < _fields.size(); i++) {
    if (_fields[i].find("filename") != st_::npos) {
      _binFiles.push_back(_fields[i]);
      _fields.erase(_fields.begin() + i);
    }
  }
  size_t pos = _fields[_fields.size() - 1].find(_endBoundary);
  if (_binFiles.size() && pos != st_::npos)
    _binFiles[_binFiles.size() - 1] =
        _binFiles[_binFiles.size() - 1].substr(0, pos);
  // return (!_binFiles.size()) ? makeResponse(201) : parseFiles();
}

void post::makeResponse(int code) { std::cout << code << std::endl; }

void post::parseFiles() {
  for (size_t i = 0; i < _binFiles.size(); i++) {
    size_t beg = _binFiles[i].find("filename");
    if (beg != st_::npos) {
      std::vector<st_> _spl = ft_split(_binFiles[i], "\r\n\r\n");
      if (_spl.size()) {
        st_ fileName = _spl[0].substr(beg + 10);
        size_t end = fileName.find("\"\r\n");
        fileName = fileName.substr(0, end);
        _binFileNames.push_back(fileName);
        st_ tmp = "";
        for (size_t i = 1; i < _spl.size(); i++) {
          tmp += _spl[i];
        }
        _binFiles[i] = tmp;
      }
    }
  }
}

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
    // ofile << std::endl;
    ofile.close();
  }
  // makeResponse(201);
}

void post::runPost(void) {
  std::string st =
      fileToStr("/Users/zmakhkha/Desktop/WebServer/Src/req_url_encoded");
  this->req = request(st);
  std::map<st_, st_> tmp_map = req.getVector();
  if (tmp_map["Content-Type"].find("multipart/form-data") != st_::npos) {
    detectBoundry();
    detectFields();
    detectBinaryFiles();
    detectDataFields();
    parseFiles();
    makeFiles();
  } else if (tmp_map["Content-Type"].find(
                 "application/x-www-form-urlencoded") != st_::npos) {
    detectUrlFields();
  } else
    makeResponse(403);
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
  // std::cout << "|" << str << "|" << std::endl;
  std::vector<st_> tmp = ft_split(str, "&");
  for (size_t i = 0; i < tmp.size(); i++) {
    std::vector<st_> ap = ft_split(tmp[i], "=");
    _urlFields.push_back(std::make_pair(ap[0], ap[1]));
  }
}
