#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <sys/_types/_ssize_t.h>
#include <unistd.h>
#include <sys/fcntl.h>


struct Transfer
{
    std::map <std::string , std::string> Headers;
    int Meth;
    bool cgi;
    int fd;
    int contentlen;
    std::string chunk;
    std::string boundary;
    std::string fileData;
    bool reading;
    std::string page1;
    std::string page2;
    Transfer()
    {
        contentlen = 0;
        boundary = "";
        reading = 1;
        cgi = 0;
    }
    void parsepage(std::string &page, Transfer &A);
    void parse2pages(std::string &page, Transfer &A);
};