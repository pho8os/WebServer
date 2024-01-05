#include "Request.hpp"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <sys/fcntl.h>
#include <sys/unistd.h>
#include <unistd.h>
#include <vector>

void to_lower(st_ &key) {
  for (size_t i = 0; i < key.length(); i++)
    key[i] = tolower(key[i]);
}
void request::clear_Obj() {
  fd = -1;
  headers.clear();
  boundary = "";
  Method_ = "";
  UniformRI = "";
  HTTPVersion_ = "";
  Parsed = false;
  isChunked = 0;
  reading = 1;
}
void request::isItinConfigFile(st_ URI) {
  int root = -1;
  std::vector<std::string> prefix;
  std::vector<Location> locations = Serv.location;
  for (int idx = 0; idx < (int)locations.size(); idx++) {
    if (locations[idx].prefix == "/")
      root = idx;
    prefix.push_back(locations[idx].prefix);
  }
  std::sort(prefix.begin(), prefix.end());
  for (int idx = prefix.size() - 1; idx >= 0; idx--) {
    if (prefix[idx] + "/" == URI.substr(0, prefix[idx].length() + 1)) {
      for (int i = 0; i < (int)locations.size(); i++)
        if (locations[i].prefix == prefix[idx])
          locate = i;
      return;
    }
  }
  // std::cout << Serv.server_name << "\n";
  if (root != -1)
    locate = root;
  else
    throw 404;
}
request::request(st_ request)
    : parseCgi(false), Parsed(true),
      chunkedHeaders(false), isChunked(false), cgi(false) {
  try {
    upPath = Serv.location[locate].up_path;

    firstParse = false;
    size_t pos = 0;
    size_t delete_ = 0;
    for (int i = 0; request[i]; i++) {
      delete_ = request.find(" ");
      if (delete_ != std::string::npos && Method_.empty())
        this->setMethod_(request.substr(0, delete_));
      else if (delete_ != std::string::npos && UniformRI.empty()) {
        if (!checkURI(request.substr(0, delete_)))
          return;
        this->setURI(request.substr(0, delete_));
      } else
        break;
      request.erase(0, delete_ + 1);
    }
    if (getMethod_() != "POST" && getMethod_() != "GET" &&
        getMethod_() != "DELETE")
      throw 501;
    delete_ = request.find("\r\n");
    if (delete_ != std::string::npos)
      setVersion(request.substr(0, delete_));
    if (getVersion() != "HTTP/1.1")
      throw 505;
    request.erase(0, delete_ + 2);
    FillHeaders_(request);
    if ((pos = headers["content-type"].find("boundary=")) !=
        std::string::npos) {
      boundary = headers["content-type"].substr(pos + 9);
    }
    KeepAlive = headers["connection"] == "keep-alive";
  } catch (int code_) {
    reading = 0;
    code = code_;
    Parsed = false;
  }
}

int request::CheckForBody() {
  st_ len;
  size_t contentL_ = 0;
  len = headers["content-length"];
  std::stringstream ss(len);
  ss >> contentL_;
  if ((headers["content-length"].empty() || contentL_ <= 0) && getMethod_() == "POST")
    throw 400;
  (!headers["transfer-encoding"].empty() && headers["transfer-encoding"] != "chunked" && getMethod_() == "POST") ? throw 501 : contentL_ = 0;
  return 1;
}

const Map &request::getVector(void) { return headers; }
st_ trimString(st_ sub) {
  int i;
  for (i = 0; sub[i] && sub[i] == ' '; i++)
    ;
  return &sub[i];
}
bool request::FillHeaders_(st_ request_) {
  size_t p = 0;
  firstParse = true;
  for (int i = 0; request_.substr(0, 2) != "\r\n" && !request_.empty(); i++) {
    size_t found_it = request_.find(":");
    if (found_it != std::string::npos) {
      st_ key = trimString(request_.substr(0, found_it));
      to_lower(key);
      request_.erase(0, found_it + 2);
      size_t found_end = request_.find("\r\n");
      if (found_end == std::string::npos || key.empty())
        break;
      st_ value = trimString(request_.substr(0, found_end));
      request_.erase(0, found_end + 2);
      headers[key] = value;
    }
    // else
    //   throw 400;
  }
  CheckForBody();
  if (getMethod_() == "GET") {
    headers["content-type"] = "";
    headers["content-length"] = "";
  }
  if (getMethod_() == "POST")
    p = headers["content-type"].find("boundary=");
  if (p != st_::npos && getMethod_() == "POST")
    boundary = headers["content-type"].substr(p + 9);
  return true;
}
bool request::checkURI(st_ URI) {
  if (URI.length() > 2048)
    throw 414;
  st_ Allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
                "-._~:/?#[]@!$&'()*+,;=%";
  for (int i = 0; URI[i]; i++) {
    int check = 0;
    for (; Allowed[check]; check++)
      if (Allowed[check] == URI[i])
        break;
    if (Allowed[check] == '\0')
      throw 400;
  }
  return true;
}

request::~request(void) { close(tmpBodyFd);}
bool request::getConnection(void) { return KeepAlive; }
void request::setMethod_(std::string Method_) { this->Method_ = Method_; }
void request::setURI(std::string URI) { this->UniformRI = URI; }
void request::setVersion(std::string version) { this->HTTPVersion_ = version; }

std::string &request::getVersion(void) { return HTTPVersion_; }
std::string &request::getURI(void) { return UniformRI; }
std::string &request::getMethod_(void) { return Method_; }

bool request::getBoolean(void) { return Parsed; }
size_t request::getCode(void) { return code; }
st_ request::getBoundary(void) { return boundary; }

int request::hextodec(const std::string &s) {
  int ret;
  int hex;

  ret = 0;
  hex = 1;
  for (int i = (int)s.size() - 1; i >= 0; i--) {
    if (std::isdigit(s[i]))
      ret += (s[i] - 48) * hex;
    else if (s[i] >= 'A' && s[i] <= 'F')
      ret += (s[i] - 55) * hex;
    else if (s[i] >= 'a' && s[i] <= 'f')
      ret += (s[i] - 87) * hex;
    else
      return -1;
    hex *= 16;
  }
  return (ret);
}

void request::execboundary(std::string s, std::string boundary) {
  if (s.find(boundary) != std::string::npos) {
    s = s.substr(s.find(boundary) + boundary.size() + 2, s.size());
  }
}

void request::parseboundary(std::string chunk) {
  std::string line = chunk.substr(0, chunk.find("\r\n"));
  chunk.erase(0, line.length() + 2);
  if (isChunked && chunk.find(boundary + "--") != st_::npos) {
    reading = 0;
    return;
  }
  if (line == boundary + "--") {
    reading = 0;
    return;
  }
  size_t pos = chunk.find("filename=\"");
  if (pos != std::string::npos) {
    std::string file = chunk.substr(pos + 10, (chunk.find("\"\r\n") - pos - 10));
    if (fd > 0)
      close(fd);
    std::string filename = upPath + file;
    fd = open(filename.c_str(), O_CREAT | O_RDWR, 0644);
    if (page1.empty() && page2.empty())
    {
      chunk.erase(0, chunk.find("\r\n\r\n") + 4);
      chunk = chunk.substr(0, chunk.find(boundary + "--") - 4 );
      write(fd, chunk.c_str(), chunk.length());
      reading = 0;
      page1 = "";
      page2 = "";
      chunk = "";
      return;
      // close(fd);
    }
  }
}

void request::parsechunk(std::string &chunk) {
  if (chunk.find(boundary) != std::string::npos && !cgi)
    parseboundary(chunk);
  else {
    write(fd, chunk.c_str(), chunk.length());
  }
}

void request::parseheaders(std::string &page) {
  size_t pos = page.find("\r\n\r\n");
  
  std::string headers = page.substr(0, pos + 2);
  page.erase(page.begin(), page.begin() + pos + 4);
}

bool request::validboundary(std::string tmp) {
  size_t pos = tmp.find(boundary);
  if (pos == std::string::npos)
    return false;

  std::string chunk = tmp.substr(0, pos);
  tmp.erase(0, pos);

  pos = tmp.find("\r\n\r\n");
  if (pos == std::string::npos &&
      tmp.substr(0, boundary.length() + 2) != boundary + "--")
    return false;
  write(fd, chunk.c_str(), chunk.length() - 4);

  std::string bounds = tmp.substr(0, pos);
  parseboundary(bounds);
  std::string chunk2 = tmp.substr(pos + 4, tmp.length());
  page1 = chunk2;
  pos = page1.find(boundary);
  page2 = "";
  return true;
}

void request::parseSimpleBoundary(std::string &page) {
  if (!parseboundaryHed)
    parseheaders(page);
  parseboundaryHed  = true;
    if (page.find(boundary + "--") != st_::npos && page1.empty()) {
    parseboundary(page);
    reading = 0;
    return;
    }
  if (page1.empty()) {
      return (page1 = page, (void)0);
  }
  if (page2.empty())
    page2 = page;
  if (!validboundary(page1 + page2)) {
    write(fd, page1.c_str(), page1.length());
    std::swap(page1, page2);
    page2 = "";
  }
}

void request::parseMe(st_ request) {
    firstParse = true;
    chunklen = 0;
    parseboundaryHed = false;
    // size_t	pos = 0;
    size_t delete_ = 0;
    for (int i = 0; request[i]; i++) {
      delete_ = request.find(" ");
      if (delete_ != std::string::npos && Method_.empty())
        this->setMethod_(request.substr(0, delete_));
      else if (delete_ != std::string::npos && UniformRI.empty()) {
        if (!checkURI(request.substr(0, delete_)))
          return;
        this->setURI(request.substr(0, delete_));
      }

      else
        break;
      request.erase(0, delete_ + 1);
    }

    delete_ = request.find("\r\n");
    if (delete_ != std::string::npos)
      setVersion(request.substr(0, delete_));
    if (getMethod_() != "POST" && getMethod_() != "GET" && getMethod_() != "DELETE")
      throw 501;
    if (getVersion() != "HTTP/1.1")
      throw 505;
    request.erase(0, delete_ + 2);
    FillHeaders_(request);
    KeepAlive = headers["Connection"] == "keep-alive";
}

// request::request() {
//   st_ tmp = cgiBodyStr;
//   while ((!access(tmp.c_str(), F_OK)))
//     tmp += "_";
//   tmpBodyFd= open(tmp.c_str(), O_CREAT | O_RDWR | O_APPEND, 0777);
//   cgi = false;
//   page1 = "";
//   page2 = "";
//   cgiReady = false;
//   Parsed = true;
//   reading = true;
//   firstParse = false;
//   contentlen = 0;
// }
request::request() {
  static int cgif;
  std::string file = std::string(cgiBodyStr) + std::to_string(cgif++);
  tmpBodyFd= open(file.c_str(), O_CREAT | O_RDWR, 0777);
  cgiBodyPath = file;
  cgi = false;
  page1 = "";
  page2 = "";
  cgiReady = false;
  Parsed = true;
  reading = true;
  firstParse = false;
  contentlen = 0;
}

bool request::getReadStat(void) const { return this->reading; }
void request::fillCgiBodyNb(const st_ &data) {
  st_ page = data;
  if (!parseCgi)
    parseheaders(page);
  size_t ln, rd;
  ln = stoi(headers["content-length"]);
  int fd = open(cgiBodyPath.c_str(), O_APPEND | O_RDWR | O_CREAT, 0644);
  if (fd < 0) {
    perror(st_(st_("Could not create : ") + cgiBodyPath.c_str()).c_str());
    return (reading = false, void(0));
  }
  rd = write(fd, page.c_str(), page.length());
  ln -= rd;
  if (ln <= 0) {
    cgiReady = true;
  }
  parseCgi = true;
  close(fd);

}

void request::fillCgiBody(const st_ &data) {
  st_ page = data;
  if (!parseCgi)
    parseheaders(page);
  int fd = open(cgiBodyPath.c_str(), O_APPEND | O_RDWR | O_CREAT, 0644);
  if (fd < 0) {
    perror(st_(st_("Could not create : ") + cgiBodyPath.c_str()).c_str());
    return (reading = false, void(0));
  }
  write(fd, page.c_str(), page.length());
  if (page.find(boundary + "--") != st_::npos) {
    cgiReady = true;
  }
  parseCgi = true;
  close(fd);

}

void request::handleCgi(const st_ &data) {
  st_ root;
  st_ str = data;
  if (Serv.location[locate].prefix != "/")
    root = Serv.location[locate].root +
           getURI().substr(
               Serv.location[locate].prefix.length()); // change
  else
    root = Serv.location[locate].root + getURI();
  if (firstParse == false)
    parseMe(data);
  if (cgi) {
    Cgi tmp(getURI(), getMethod_(), locate, cgiResult, getVector() ,upPath, Serv);
    if (getMethod_() == "POST" && !cgiReady) {
      return;
    } else if (getMethod_() == "GET") {
      cgiReady = true;
      cgiBodyPath = "";
    }
    if (cgiReady) {
      tmp.excecCgi(cgiBodyPath);
      reading = false;
    }
  } else
    throw 404;
}

void request::chunkData(std::string &data) {
  isChunked = true;
  while (chunklen <= 0) {
    size_t pos = data.find(("\r\n"));
    if (pos == st_::npos) {
      page1 = data;
      page2 = "";
      return;
    }
    st_ line = data.substr(0, pos);
    data.erase(0, pos + 2);
    chunklen = hextodec(line);
  }
  size_t dataLen = data.length();
  if (chunklen > (int)dataLen) {
    page1 = data;
    page2 = "";
    return;
  } else {
    chunk = data.substr(0, chunklen);
    data.erase(0, chunklen);
    chunklen = 0;
    parsechunk(chunk);
  }
}

void request::parseChunked(std::string &page) {
  

  if (page.find(boundary + "--") != st_::npos && page1.empty())
  {
    // chunkData(page);
    parsechunk(page);

    reading = 0;
    return;
  }
  if (!chunkedHeaders) {
    parseheaders(page);
    chunkedHeaders = true;
  }

  if (page1.empty())
    return (page1 = page, (void)0);
  if (page2.empty())
    page2 = page;
  st_ data = page1 + page2;

  while (data.find("\r\n") != st_::npos && reading && chunklen < (int)data.length()) {
    chunkData(data);
  }
  page1 = data;
  page2 = "";
}

void request::feedMe(const st_ &data) {
  try {
    st_ str = data;
    cgiResult = cgiResStr;
    if (firstParse == false)
      parseMe(data);
    this->Serv = Config::getservconf(headers["server-name"], headers["host"]);
    isItinConfigFile(UniformRI);
    if (!Serv.location[locate].allow.Post && getMethod_() == "POST") throw 405;
    upPath = Serv.location[locate].up_path;
    if ((getURI().find(".py") != std::string::npos ||
         getURI().find(".php") != std::string::npos) &&
        (!Serv.location[locate].cgi.first.empty()))
      cgi = 1;
    if (cgi) {
      if (getMethod_() == "DELETE")
        throw 502;
      if (getMethod_() == "POST" && !cgiReady) {
        (headers["content-type"].find("boundary") != st_::npos)
            ? fillCgiBody(str)
            : fillCgiBodyNb(str);
      }
      if (getMethod_() == "GET")
        cgiReady = 1;
      if (cgiReady) {
        handleCgi(data);
      }
    } else {
      if (getMethod_() == "GET" || getMethod_() == "DELETE")
        return (reading = false, void(0));
      else if (getMethod_() == "POST" && getBoolean()) {
        if(!maxBody())  throw 413; 
        if (boundary.empty())
          throw 201;
        isChunked = headers["transfer-encoding"] == "chunked";
        isChunked ? parseChunked(str) : parseSimpleBoundary(str);
        if (reading == 0)
        {
          throw 201;
        }
      }
    }
  } catch (int code_) {
    reading = 0;
    Parsed = false;
    code = code_;
  }
}

bool request::maxBody() {
  size_t size;
  char letter = Serv.location[locate].body_size.second;
  size_t body_size = Serv.location[locate].body_size.first;
  st_ length = headers["content-length"];
  std::stringstream var(length);
  var >> size;
  if (letter == 'M')
    body_size *= M;
  if (letter == 'K')
    body_size *= K;
  if (letter == 'G')
    body_size *= G;
  return ( size <= body_size );
}

Server request::getServer()
{
  return this->Serv;
}
