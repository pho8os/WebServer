#pragma once

#include <iostream>
#include "../ConfigFile/ConfigFile.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#define st_ std::string
class request;
class Response;

class Response {	
	private :
		Config set_;
		size_t	status_code;
	public :
		Response(void);
		Response &RetResponse( request &req );
		int isItinConfigFile( st_ URI, std::vector < Server > server ) const;
		int	checkMethods( request &req, std::vector < Server > server, int idx );
		~Response(void);
};