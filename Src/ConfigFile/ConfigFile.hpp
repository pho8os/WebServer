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

struct Methods
{
    bool Get;
    bool Post;
    bool Delete;
    Methods();
};

struct Shared
{
    std::string root;
    std::string up_path;
    Methods allow;
    std::vector<std::string> index;
    std::vector<std::string> error_page;
    size_t body_size;
    std::string redirect;
};

struct Location : public Shared
{
    std::string prefix;//
    bool autoindex;//
    std::string cgi;//
};

struct Server : public Shared
{
    std::vector<Location> location;//
    std::string host;//
    std::string server_name;//
    std::vector<std::string> port;//
};

class Config
{
    private :
        std::vector <Server> server;
    public :
		std::vector <Server> getVector(void) const { return server; };
};