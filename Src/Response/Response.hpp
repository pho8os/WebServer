#pragma once

#include <iostream>
#include "../Request/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#define SERVER "saadia lm7wia"
#define st_ std::string
class request;
class Response {
	private :
		Config set_;
		int	location;
		size_t	status_code;
		st_		ret;
		std::map < int, st_ > error_codes;
	public :
		st_	directory;
		Response(void);
		void	init_TheCont_();
		Response &RetResponse( request &req );
		int isItinConfigFile( st_ URI, std::vector < Server > server ) const;
		int	checkMethods( request &req, std::vector < Server > server, int idx );
		void getPage( request &req );
		int	GETResource();
		st_	getRet();
		~Response(void);
};