#pragma once

#include "../ConfigFile/ConfigFile.hpp"
#include "../Response/Response.hpp"

#define BAD_GATEWAY 502
#define phpPath "/Users/zmakhkha/Desktop/cgi/src/cgi/cgi-bin/php-cgi"
#define pythonPath "/usr/bin/python3"
#define st_ std::string
class request;

class Cgi{
	private:
		st_ SERVER_SOFTWARE;
		st_ SERVER_NAME;
		st_ GATEWAY_INTERFACE;


		st_ _uri, _methode;
		int _location;
		st_ upload_path;

		std::vector<std::string> _envLst;
		std::map<std::string, std::string> _reqHeaders;

		std::string _CgiScriptPath;
		std::string _scriptPath;

		Config configuration;
  		bool _isPost;

		std::string _postBody;
		std::string _respPath;
		Server srv;
		Cgi();

	public:
		Cgi(st_ uri, st_ methode, int loc, st_ cgiRes, std::map<st_, st_> heads, st_ upPath, Server _srv);
		Cgi(int var);
		~Cgi();
		void setHeaders(const std::map<std::string,const std::string> headers);
		void formatHeaders();
		void setEnv();
		void headToEnv(std::string &str);
		void execute();
		void formatKey(std::string &key);
		void excecCgi(std::string bodyPath);
		void setExtraEnv();
		st_ getRespPath(void) const;
		void setUnique();

		std::string getQueryStr();
		std::pair<st_, st_> getPathQuery(st_ uri);


};