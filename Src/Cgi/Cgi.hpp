
#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <map>
#include <algorithm>

#define st_ std::string

class Cgi
{
private:
	std::map<st_, st_> _envVars; 
	std::string _type;
	std::string _php_path;
	std::string _php_cgi;
	std::string _php_src;

public:
	Cgi();
	~Cgi();
	Cgi(const Cgi &obj);
	Cgi &operator=(const Cgi &obj);
	
	void setEnvVars(std::map<st_, st_> map);

	std::string get_ress_path(std::string query);
	bool found_ressource(std::string path);
	bool execute(std::string query);
	bool makeResponse(const std::string query);
	std::string getSrcResult();
};