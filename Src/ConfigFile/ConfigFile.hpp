#pragma once

#include<iostream>
#include <map>
#include <sys/_types/_size_t.h>
#include <vector>


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

// string server_directives[] = {
//   "port",
//   "host",
//   "index",
//   "server_name",
//   "allowed_methods",
//   "root",
//   "error_page",
//   "client_max_body_size"
//   "location",
//   "upload_path",
//   "redirect"
// };


struct Location
{

};

struct Server{
    int port;
    std::string host;
    std::vector<std::string> index;

};

class Config
{
    private:
        std::vector<Server> server;
    public:
    //TODO: methods of config file
};