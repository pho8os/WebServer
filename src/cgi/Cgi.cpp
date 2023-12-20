#include "Cgi.hpp"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sys/fcntl.h>
#include <unistd.h>

Cgi::~Cgi() {}

Cgi::Cgi(st_ uri, st_ methode, int loc, st_ cgiRes, std::map<st_, st_> heads) :
_uri(uri), _methode(methode), _location(loc), _reqHeaders(heads), _respPath(cgiRes)
{
  _CgiScriptPath = configuration.getConfig()[0].location[_location].cgi.second;
  _isPost = _methode == "POST";
}

void Cgi::formatKey(std::string &key) {
  std::string tmp = "HTTP_";
  for (size_t i = 0; i < key.length(); i++) {
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
        _envLst.push_back(tmpKey + "=" + it->second + "");
  }
}


void Cgi::setExtraEnv() {
  formatHeaders();
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
  return res;
}



void Cgi::printEnv()
{
  for (size_t i = 0; i < _envLst.size(); i++)
  {
    std::cout << "[" << i << "] " << _envLst[i] << std::endl;
  }
}


void Cgi::setEnv() {

		st_ SERVER_SOFTWARE  = "SA3DYA/V1.0";
		st_ SERVER_NAME= "SA3DYA";
		st_ GATEWAY_INTERFACE= "SA3SYA_CGI/1.1";

  std::pair<st_, st_> tmp = getPathQuery(_uri);
  st_ root = configuration.getConfig()[0].location[_location].root;
  int pref_len = configuration.getConfig()[0].location[_location].prefix.length();
  _envLst.push_back("SERVER_SOFTWARE=" + SERVER_SOFTWARE + "");
  _envLst.push_back("GATEWAY_INTERFACE=" + GATEWAY_INTERFACE + "");
  _envLst.push_back("SERVER_NAME=" + SERVER_NAME + "");
  _envLst.push_back("SERVER_PROTOCOL=HTTP/1.1");
  _envLst.push_back("SERVER_PORT=" + configuration.getConfig()[0].listen.second + "");
  _envLst.push_back("REQUEST_METHOD=" + _methode + "");
  _envLst.push_back("PATH_INFO=" + configuration.getConfig()[0].location[_location].prefix + tmp.first.substr(pref_len) + "");
  _envLst.push_back("SCRIPT_NAME=" + configuration.getConfig()[0].location[_location].prefix + tmp.first.substr(pref_len) + "");
  _envLst.push_back("PATH_TRANSLATED=" + root + "/" + tmp.first.substr(pref_len) + "");
  _scriptPath = root + "/" + tmp.first.substr(pref_len);
  _envLst.push_back("QUERY_STRING=" + tmp.second + "");
  if (access((root + "/" + tmp.first.substr(pref_len) + "").c_str(), F_OK) == -1)
    throw 404;
  _envLst.push_back("UPLOAD_DIRECTORY=./upload");
  _envLst.push_back("REDIRECT_STATUS=200");
  this->setUnique();
}

void Cgi::setUnique()
{
  _envLst.push_back("CONTENT_LENGTH=" + _reqHeaders["content-length"]);
  _envLst.push_back("CONTENT_TYPE=" + _reqHeaders["content-type"]);
  _reqHeaders.erase(_reqHeaders.find("content-length"));
  _reqHeaders.erase(_reqHeaders.find("content-type"));

}

void Cgi::excecCgi(std::string bodyPath)
{
  this->_postBody = bodyPath;
  std::cout << "--------->|" << _postBody << std::endl;
  _isPost = bodyPath.length() != 0;
  setEnv();
  setExtraEnv();
  printEnv();
  execute();
}


void Cgi::execute() {
  if (!_scriptPath.length())
    throw (501);
  pid_t pid = fork();

  if (pid == 0) {
    char *envp[_envLst.size() + 1];
    for (std::size_t i = 0; i < _envLst.size(); ++i) {
      envp[i] = const_cast<char *>(_envLst[i].c_str());
    }
    envp[_envLst.size()] = NULL;
    char *argv[] = {const_cast<char *>(_CgiScriptPath.c_str()),
                    const_cast<char *>(_scriptPath.c_str()), NULL};
    int fd = open(_respPath.c_str(), O_CREAT | O_RDWR, 0644);
    if (fd < 0)
      perror("open : ");
    std::cout << "-------->|||||||\n" << _respPath;
    FILE *out = freopen(_respPath.c_str(), "w", stdout);
    if (_isPost) {
      std::cout << "asdlhgbdhjsbhbdfadf\n\n\n";
      FILE *in = freopen(_postBody.c_str(), "r", stdin);
      if (in == nullptr) {
        perror("freopen : ");
      }
    }
    if (out == nullptr) {
      perror("freopen : ");
    }
    execve(argv[0], argv, envp);
    perror("execve");
  } else if (pid > 0) {
    waitpid(pid, nullptr, 0);
  } else {
    perror("fork");
  }
}


st_ Cgi::getRespPath(void) const
{
  return this->_respPath;
}
