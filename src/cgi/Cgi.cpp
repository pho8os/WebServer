#include "Cgi.hpp"
#include <cctype>
#include <cstdio>
#include <ios>
#include <string>
#include <type_traits>
#include <utility>
#define BAD_GATEWAY 502

Cgi::Cgi() {
  _CgiScriptPath = "Cgi/python-Cgi";
  _scriptPath = "42-Cgi/Cgi/script.py";
}

Cgi::Cgi(int var) {

  _headers.push_back("SERVER_NAME=");
  _headers.push_back("SERVER_SOFTWARE=AMN_AKA_SA3DYA");
  _headers.push_back("PATH_INFO=");
  _headers.push_back("UPLOAD_DIRECTORY=./upload");
  switch (var) {
  case PY:
    _headers.push_back("SCRIPT_NAME=Cgi/python-Cgi");
    _CgiScriptPath = "42-Cgi/Cgi/python-Cgi";
    _scriptPath = "42-Cgi/Cgi/script.py";
    break;
  case PHP:
    _headers.push_back("REDIRECT_STATUS=200");
    _headers.push_back("SCRIPT_NAME=/Cgi/php-Cgi");
    _CgiScriptPath = "42-Cgi/Cgi/php-Cgi";
    _scriptPath = "Cgi/script.php";
    break;
  default:
    throw(BAD_GATEWAY);
  }
}


Cgi::~Cgi() {}
// void Cgi::setEnv() {

//   //   /from the request
//   // _headers.push_back("QUERY_STRING=key01=val01&key02=val02");
//   _headers.push_back("CONTENT_LENGTH=270");
//   _headers.push_back("COOKIE=");
//   _headers.push_back("USER_AGENT=PostmanRuntime/7.33.0");
//   _headers.push_back("ACCEPT=*/*");
//   _headers.push_back("POSTMAN_TOKEN=592ecb38-e70a-46c0-911b-e33b975aec7f");
//   _headers.push_back("ACCEPT_ENCODING=gzip, deflate, br");
//   _headers.push_back("CONNECTION=keep-alive");
//   _headers.push_back(
//       "CONTENT_TYPE=multipart/form-data; "
//       "boundary=--------------------------870504741970283687111411");
//   _headers.push_back("REQUEST_METHOD=POST");
// }

void Cgi::execute() {
  pid_t pid = fork();

  if (pid == 0) {
    char *envp[_headers.size() + 1];
    for (std::size_t i = 0; i < _headers.size(); ++i) {
      envp[i] = const_cast<char *>(_headers[i].c_str());
    }
    envp[_headers.size()] = nullptr;

    char *argv[] = {const_cast<char *>(_CgiScriptPath.c_str()),
                    const_cast<char *>(_scriptPath.c_str()), nullptr};
    FILE *out = freopen("output.txt", "w", stdout);
    if (_isPost) {
      FILE *in = freopen("post_body", "r", stdin);
      if (in == nullptr) {
        perror("freopen");
        exit(EXIT_FAILURE);
      }
    }
    if (out == nullptr) {
      perror("freopen");
      exit(EXIT_FAILURE);
    }
    execve(_CgiScriptPath.c_str(), argv, envp);
    printf("This sentence is redirected to a file.");
    fclose(stdout);
    perror("execve");
    exit(EXIT_FAILURE);
  } else if (pid > 0) { // Parent process
    waitpid(pid, nullptr, 0);
  } else {
    perror("fork");
  }
}

void Cgi::formatKey(std::string &key) {
  std::string tmp = "HTTP_";
  for (int i = 0; i < key.length(); i++) {
    if (key[i] == '-')
      key[i] = '_';
    key[i] = std::toupper(key[i]);
  }
    key = tmp + key;
}

void Cgi::formatHeaders() {
  std::string tmpKey;
  for (std::map<std::string, std::string>::iterator it = _reqHeaders.begin();
       it != _reqHeaders.end(); it++) {
        tmpKey = it->first;
        formatKey(tmpKey);
        _envLst.push_back(tmpKey + "=\"" + it->second + "\"");
  }
}


void Cgi::setExtraEnv(request &req) {
  std::string key = "content-lenght";
  _reqHeaders = req.getVector();
  if (_reqHeaders.find(key) != _reqHeaders.end())
    _reqHeaders.erase(_reqHeaders[key]);
  key= "content-type";
  if (_reqHeaders.find(key) != _reqHeaders.end())
    _reqHeaders.erase(_reqHeaders[key]);
  formatHeaders();
}

void Cgi::setEnv(request &req) {

		st_ SERVER_SOFTWARE  = "SA3DYA/V_1_0";
		st_ SERVER_NAME= "SA3DYA";
		st_ GATEWAY_INTERFACE= "SA3DYA/CGI";

  std::pair<st_, st_> tmp = getPathQuery(req.getURI());
  req.isItinConfigFile(req.getURI(), configuration.getConfig());
  st_ root = configuration.getConfig()[0].location[req.getLoc()].root;
  int prefix = configuration.getConfig()[0].location[req.getLoc()].prefix.length();

  _envLst.push_back("GATEWAY_INTERFACE=" + GATEWAY_INTERFACE + "\"");
  _envLst.push_back("SERVER_SOFTWARE=" + SERVER_SOFTWARE + "\"");
  _envLst.push_back("SERVER_NAME=" + SERVER_NAME + "\"");
  
  
  
  _envLst.push_back("SERVER_NAME=\"" + req.getURI() + "\"");



  _envLst.push_back("SERVER_PROTOCOL=\"" + req.getURI() + "\"");
  _envLst.push_back("SERVER_PORT=\"" + configuration.getConfig()[0].listen.second + "\"");
  _envLst.push_back("REQUEST_METHOD=\"" + req.getMethod_() + "\"");
  _envLst.push_back("PATH_INFO=\"" + tmp.first + "\"");
  _envLst.push_back("PATH_TRANSLATED=\"" + root + req.getURI().substr(prefix) + "\"");
  _envLst.push_back("SCRIPT_NAME=\"" + req.getURI() + "\"");
  _envLst.push_back("QUERY_STRING=\"" + tmp.second + "\"");
  _envLst.push_back("CONTENT_TYPE=\"" + req.getMethod_() + "\"");
  _envLst.push_back("CONTENT_LENGTH=\"" + req.getMethod_() + "\"");
}

std::pair<st_, st_> Cgi::getPathQuery(st_ uri)
{
  std::pair<st_, st_> res;
  size_t pos = uri.find("?");
  if (pos != st_::npos)
  {
    res.first = uri.substr(0, pos);
    res.second = uri.substr(pos + 1, uri.length());
  }
  else
  {
    res.first = uri;
    res.second = "";
  }
  std::cout << "----------->" << res.first << std::endl;
  std::cout << "----------->" << res.second << std::endl;
  return res;
}



void Cgi::printEnv()
{
  for (int i = 0; i < _envLst.size(); i++)
  {
    std::cout << "[" << i << "] " << _envLst[i] << std::endl;
  }
}


std::string Cgi::getQueryStr(request req)
{
  return "";
}

void Cgi::excecCgi(request req, std::string bodyPath)
{
  _isPost = bodyPath.length() != 0;
  setEnv(req);
  setExtraEnv(req);
  printEnv();
}
