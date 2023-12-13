#pragma once

#include <iostream>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include "../Request/Request.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#define SERVER "sa3dia"
#define st_ std::string
class request;

struct DirectoryInfo {
	st_ 		path;
	st_			first_path;
	struct stat stru_t;
	struct dirent *directory;
	std::vector < st_ > directories;
	std::vector < st_ > files;
	DIR			*dir;
};

struct fd_
{
    struct stat statbuf;
    off_t offset;
    off_t *len;
    fd_(int fd){
        fstat(fd, &statbuf);
    }
};

class Response {
	private :
		int		fd;
		Config set_;
		st_		ret;
		bool	loc;
		char	*buffer;
		int		location;
		DirectoryInfo	inf;
		size_t	status_code;
		std::map < st_, st_ > text_types;
		std::map < int, st_ > error_codes;
	public :
		bool headersent;
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
		void	DeleteContent( request &req, st_ path );
		int		Fill_Resp( request &req, st_ root );
		void	deleteFile( request &req );
		void	deleteDir( request &req );
		void	is_file( st_ path, request &req );
		void	is_dir( st_ root, std::vector < Server > res, request &req );
		void	setRet( st_ &ret );
		void	content_types();
		void	openDir( st_ path, request &req );
		st_		getRet();
		~Response(void);
};