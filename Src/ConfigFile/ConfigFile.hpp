#pragma once

#include<iostream>
#include <string>
#include <sys/_types/_size_t.h>
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
    std::string prefix;
    bool autoindex;
    std::map<std::string, std::string> cgi;
};

struct Server : public Shared
{
    std::vector<Location> location;
    std::pair<std::string, int> listen;
    std::string server_name;
};

class Config
{
    private:
        static std::vector<Server> server;
    public:

        const std::vector<Server> getConfig() const;
};
std::vector<Server> parseconf(const std::string &path);
#include "ConfigFile.tpp"