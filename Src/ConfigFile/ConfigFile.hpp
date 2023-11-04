#pragma once

#include<iostream>
#include <string>
#include <sys/_types/_size_t.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <ostream>
#include <sstream>
#include <stdexcept>




// string location_directives[] = {
//   "prefix",
//   "root",
//   "upload_path",
//   "autoindex",
//   "redirect",
//   "allowed_methods",
//   "index",
//   "cgi_path",
//   "error_page",
//   "client_max_body_size"
// };



struct Location
{
    std::string prefix;
    std::string root;
    std::string up_path;
    bool autoindex;
    bool methods[3];
    std::vector<std::string> index;
    std::string cgi;
    std::vector<std::string> error_page;
    size_t body_size;
    std::string redirect;

};

struct Server{
   
    std::vector<Location> location; //"location", "index", "error_page", "up_path", "port", "root", "host", "server_name", "max_body_size", "rederict", "allow"
    std::vector<std::string> index;
    std::vector<std::string> error_page;
    std::string up_path;
    std::vector<std::string> port;
    std::string root;
    std::string host;
    std::string server_name;
    size_t max_body_size;
    std::string redirect;
    bool allow[3];
    void parslocation(std::deque<std::string> &file, Server &serv);
    void parsindex(std::deque<std::string> &file, Server &serv);
    void parsError_page(std::deque<std::string> &file, Server &serv);
    void parsUp_Path(std::deque<std::string> &file, Server &serv);
    void parsPort(std::deque<std::string> &file, Server &serv);
    void parsRoot(std::deque<std::string> &file, Server &serv);
    void parsHost(std::deque<std::string> &file, Server &serv);
    void parsServer_name(std::deque<std::string> &file, Server &serv);
    void parsMax_Body_size(std::deque<std::string> &file, Server &serv);
    void parsRederict(std::deque<std::string> &file, Server &serv);
    void parsMethods(std::deque<std::string> &file, Server &serv);
    void Error(std::deque<std::string> &file, Server &serv);

};

class Config
{
    private:
        std::vector<Server> server;
    public:
    //TODO: methods of config file
};