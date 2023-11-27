#pragma once

#include <iostream>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include "../Request/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#include "../Request/post/post.hpp"
#define SERVER "sa3dia"
#define st_ std::string
class request;

class Response {
	private :
		Config set_;
		st_		ret;
		bool	loc;
		int	location;
		char	*buffer;
    	post    ob_post;
		size_t	status_code;
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
		bool	index_file( request &req, st_ path );
		void	GETResource( request &req );
		void	DELResource( request &req );
		int	Fill_Resp( request &req, st_ root );
		void	is_file( st_ path, request &req );
		void	is_dir( st_ root, std::vector < Server > res, request &req );
		void	setRet( st_ &ret );
		st_		getRet();
		~Response(void);
};