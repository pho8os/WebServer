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
    std::string fileData;
    bool reading;
    std::string page1;
    std::string page2;
    Transfer()
    {
        contentlen = 0;
        boundary = "----------------------------683634263385122688561624";
        reading = 1;
        cgi = 0;
        fd = open("cgi", O_CREAT | O_RDWR, 0644);
    }

};