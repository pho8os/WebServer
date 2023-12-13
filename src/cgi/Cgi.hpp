#pragma once
#include "../Request/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <algorithm>
#include <unistd.h>

#define SERVER_SOFT "AMN / 1.1 AKA SA3DYA"
#define PHP -1
#define PY -2
#define st_ std::string


class request;
class Cgi{
	private:
		st_ SERVER_SOFTWARE;
		st_ SERVER_NAME;
		st_ GATEWAY_INTERFACE;
		std::vector<std::string> _headers;
		std::map<std::string, std::string> _env;
		std::vector<std::string> _envLst;
		std::string _CgiScriptPath;
		std::string _scriptPath;
		std::map<std::string, std::string> _reqHeaders;
		Config configuration;
  		bool _isPost;

	public:
		Cgi();
		Cgi(int var);
		~Cgi();
		void setHeaders(const std::map<std::string, const std::string> headers);
		void formatHeaders();
		void setEnv(request &req);
		void headToEnv(std::string &str);
		void execute();
		void formatKey(std::string &key);
		void printEnv();
		void excecCgi(request req, std::string bodyPath);
		std::string getQueryStr(request req);
		void setExtraEnv(request &req);
		std::pair<st_, st_> getPathQuery(st_ uri);


};