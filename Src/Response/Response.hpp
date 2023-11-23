#pragma once

#include <iostream>
#include <ctime>
#include <dirent.h>
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
		bool	loc;
		std::map < int, st_ > error_codes;
	public :
		st_	directory;
		Response(void);
		void	init_TheCont_();
		st_	Create_DefPage();
		Response &RetResponse( request &req );
		void isItinConfigFile( st_ URI, std::vector < Server > server );
		int	checkMethods( request &req, std::vector < Server > server, int idx );
		void getPage( request &req );
		void	Set_Up_Headers( st_ &ret, request &req, st_ body );
		bool	index_file( int i, request &req );
		int	GETResource( request &req );
		int	Fill_Resp( request &req, st_ root );
		st_	getRet();
		~Response(void);
};