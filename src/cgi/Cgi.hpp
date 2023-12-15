#pragma once

#include "../ConfigFile/ConfigFile.hpp"
#include "../Response/Response.hpp"

#define BAD_GATEWAY 502
#define phpPath "/Users/zmakhkha/Desktop/WebServer/src/Cgi/cgi-bin/php-cgi"
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

		std::vector<std::string> _envLst;
		std::map<std::string, std::string> _reqHeaders;

		std::string _CgiScriptPath;
		std::string _scriptPath;

		Config configuration;
  		bool _isPost;

		std::string _postBody;
		std::string _respPath;
		Cgi();

	public:
		Cgi(st_ uri, st_ methode, int loc, std::map<st_, st_> heads);
		Cgi(int var);
		~Cgi();
		void setHeaders(const std::map<std::string, const std::string> headers);
		void formatHeaders();
		void setEnv();
		void headToEnv(std::string &str);
		void execute();
		void formatKey(std::string &key);
		void printEnv();
		void excecCgi(std::string bodyPath);
		std::string getQueryStr();
		void setExtraEnv();
		std::pair<st_, st_> getPathQuery(st_ uri);
		st_ getRespPath(void) const;



};