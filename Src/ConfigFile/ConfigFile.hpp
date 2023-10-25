#pragma once

#include<iostream>
#include <map>
#include <sys/_types/_size_t.h>
#include <vector>


class Location
{
    private:
        std::string prefix;
        std::string cgi_path;
    protected:
        std::string server_name;
        std::string root;
        std::string upload_path;
        size_t max_body_size;
        std::vector<std::string> index;
        std::vector<std::string> error_pages;
        std::vector<std::string> allow_methods;
    public:
        void setPrefix(std::string &Prefix);
        void setRoot(std::string &Root);
        void setUp_path(std::string &Up_path);
        void setMax_body_s(std::string &Max_body_s);
        void setIndex(std::string &Index);
        void setError_pages(std::string &Error_pages);
        void setAllowMeth(std::string &AllowMeth);
};

class ServBody : public Location
{
    private:
        std::string port;
        std::string host;
        std::vector<Location> location;
    public:
        



};


class Config
{
    private:
        std::vector<ServBody> server;
    public:
    //TODO: methods of config file
};