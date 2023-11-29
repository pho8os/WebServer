#ifndef CGI_HPP
#define CGI_HPP
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <unistd.h>

#define SERVER_SOFT "AMN / 1.1 AKA SA3DYA"
#define PHP -1
#define PY -2

class cgi{
	private:
		std::vector<std::string> _headers;
		std::map<std::string, std::string> _env;
		std::string _cgiScriptPath;
		std::string _scriptPath;
  		bool _isPost;

	public:
		cgi();
		cgi(int var);
		~cgi();
		void setHeaders(const std::map<std::string, const std::string> headers);
		void setEnv();
		void headToEnv(std::string &str);
		void execute();
};
#endif