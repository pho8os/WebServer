#pragma once

#include <iostream>
#include <ctime>
#include "../Request/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#define SERVER "sa3dia"
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
		st_	Create_DefPage();
		Response &RetResponse( request &req );
		int isItinConfigFile( st_ URI, std::vector < Server > server ) const;
		int	checkMethods( request &req, std::vector < Server > server, int idx );
		void getPage( request &req );
		void	Set_Up_Headers( st_ &ret, request &req, st_ body );
		bool	index_file( int i, request &req );
		int	GETResource( request &req );
		st_	getRet();
		~Response(void);
};