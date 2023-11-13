#include "ConfigFile.hpp"

void	Config::init_vars__() {
	Server	inst_;
	Location	inst_1;
	inst_1.prefix = "/";
	inst_1.allow.Get = false;
	inst_1.allow.Delete = true;
	inst_1.allow.Post = true;
	server.push_back(inst_);
	server[0].location.push_back(inst_1);
	server[0].location[0].cgi = "py /Cgi/main.py";
	server[0].location[0].autoindex = true;
	server[0].location[0].root = "/usr/mnassi/";
	server[0].location[0].body_size = 3;
	server[0].host = "127.0.0.1";
}