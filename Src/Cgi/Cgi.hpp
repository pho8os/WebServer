#pragma once
#include "../ConfigFile/ConfigFile.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../Server/Server.hpp"

#define PY  0
#define PHP 1

class Server;
class request;
class Response;
class Config;

#define st_ std::string

class Cgi {
private:
  st_ _execPath;
  st_ _postbody;
  int fdPostBody;
  bool _type;

public:
  Cgi();
  ~Cgi();
  Cgi(const Cgi &obj);
  Cgi &operator=(const Cgi &obj);
  void setPostBody(const st_ &path);
};