#pragma once



#include <cstddef>
#include <iostream>
#include <map>
#include <fstream>


struct Transfer
{
    std::map<std::string , std::string> Headers;
    int Meth;
    bool cgi;
    int fd;
    int contentlen;
    std::string chunk;
    std::string boundary;
    Transfer()
    {
        contentlen = 0;
        chunk = "";
    }

};