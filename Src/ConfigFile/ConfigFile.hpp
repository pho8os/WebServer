#pragma once

#include<iostream>
#include <string>
#include <sys/_types/_size_t.h>
#include <vector>
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
   
    std::vector<Location> location;
    std::vector<std::string> index;
    std::vector<std::string> error_page;
    std::string up_path;
    std::vector<std::string> port;
    std::string root;
    std::string host;
    std::string server_name;
    size_t body_size;
    std::string redirect;
    bool methods[3];

};

class Config
{
    private:
        std::vector<Server> server;
    public:
    //TODO: methods of config file
};