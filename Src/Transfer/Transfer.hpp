#pragma once



#include <cstddef>
#include <iostream>
#include <map>
#include <fstream>
#include <sys/fcntl.h>


struct Transfer
{
    std::map<std::string , std::string> Headers;
    int Meth;
    bool cgi;
    int fd;
    int contentlen;
    std::string chunk;
    std::string boundary;
    bool reading;
    Transfer()
    {
        contentlen = 0;
        chunk = "";
        reading = 1;
        cgi = 0;
        fd = open("cgi", O_CREAT | O_RDWR, 0644);
    }

};