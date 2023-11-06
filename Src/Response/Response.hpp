#pragma once

#include <iostream>
#include "../ConfigFile/ConfigFile.hpp"
#include "../Request/Request.hpp"
class Request;

class Response {	
	private :
		Config set_;
	public :
		Response(void);
		Response &RetResponse( request &req );
		bool isItinConfigFile( st_ URI, std::vector < Server > server ) const;
		~Response(void);
};