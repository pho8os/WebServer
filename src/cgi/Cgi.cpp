#include "Cgi.hpp"
#include <cstdio>
#include <type_traits>

cgi::cgi() {
  _cgiScriptPath = "cgi/python-cgi";
  _scriptPath = "42-cgi/cgi/script.py";
}

cgi::cgi(int var) {

  _headers.push_back("SERVER_NAME=");
  _headers.push_back("SERVER_SOFTWARE=AMN_AKA_SA3DYA");
  _headers.push_back("PATH_INFO=");
  _headers.push_back("UPLOAD_DIRECTORY=./upload");
  switch (var) {
  case PY:
    _headers.push_back("SCRIPT_NAME=cgi/python-cgi");
    _cgiScriptPath = "42-cgi/cgi/python-cgi";
    _scriptPath = "42-cgi/cgi/script.py";
    break;
  case PHP:
    _headers.push_back("REDIRECT_STATUS=200");
    _headers.push_back("SCRIPT_NAME=/cgi/php-cgi");
    _cgiScriptPath = "42-cgi/cgi/php-cgi";
    _scriptPath = "cgi/script.php";
    break;
  default:
    throw(502);
  }
}
cgi::~cgi() {}
void cgi::setEnv() {

  //   /from the request
  // _headers.push_back("QUERY_STRING=key01=val01&key02=val02");
  _headers.push_back("CONTENT_LENGTH=270");
  _headers.push_back("COOKIE=");
  _headers.push_back("USER_AGENT=PostmanRuntime/7.33.0");
  _headers.push_back("ACCEPT=*/*");
  _headers.push_back("POSTMAN_TOKEN=592ecb38-e70a-46c0-911b-e33b975aec7f");
  _headers.push_back("ACCEPT_ENCODING=gzip, deflate, br");
  _headers.push_back("CONNECTION=keep-alive");
  _headers.push_back(
      "CONTENT_TYPE=multipart/form-data; "
      "boundary=--------------------------870504741970283687111411");
  _headers.push_back("REQUEST_METHOD=POST");
}

void cgi::execute() {
  pid_t pid = fork();

  if (pid == 0) {
    char *envp[_headers.size() + 1];
    for (std::size_t i = 0; i < _headers.size(); ++i) {
      envp[i] = const_cast<char *>(_headers[i].c_str());
    }
    envp[_headers.size()] = nullptr;

    char *argv[] = {const_cast<char *>(_cgiScriptPath.c_str()),
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
    execve(_cgiScriptPath.c_str(), argv, envp);
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