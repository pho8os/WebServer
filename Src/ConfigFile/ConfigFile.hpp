#pragma once

#include<iostream>
#include <string>
//#include <sys/_types/_size_t.h>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


// #define uploadPath "/goinfre/zmakhkha/upload/"
#define cgiResStr "/tmp/res"
#define cgiBodyStr "/tmp/body"

struct Methods
{
    bool Get;
    bool Post;
    bool Delete;
    Methods();
    bool empty(); 
};

struct Shared
{
    std::string root;
    std::string up_path;
    Methods allow;
    std::vector<std::string> index;
    std::map<int, std::string> error_page;
    std::pair<size_t, char> body_size;
    std::pair<int, std::string> redirect;
    Shared();
};

struct Location : public Shared
{
    Location() {
        autoindex = false;
    }
    std::string prefix;
    bool autoindex;
    std::pair<std::string, std::string> cgi;
};

struct Server : public Shared
{
    std::vector<Location> location;
    std::pair<std::string, std::string> listen;
    std::string server_name;
};

class Config
{
    private:
        static std::vector<Server> server;
    public:
        static void setConfig(std::vector<Server> serv);
        static const std::vector<Server> &getConfig();
        void print_config() const;
        static Server getservconf(std::string server_name, std::string host);
};
std::vector<Server> parseconf(const std::string &path);
#include "ConfigFile.tpp"