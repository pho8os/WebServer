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
	DIR			*dir;
	st_ 		path;
	st_			first_path;
	struct stat stru_t;
	struct dirent *directory;
	std::vector < st_ > directories;
	std::vector < st_ > files;
};

struct fd_
{
    off_t len;
    off_t offset;
    struct stat statbuf;
	fd_( void ){};
    fd_(int fd){
        fstat(fd, &statbuf);
    }
	fd_ &operator=( const fd_ &obj ) {
		len = obj.len;
		offset = obj.offset;
		statbuf = obj.statbuf;
		return *this;
	}
};

class Response {
	private :

		int				fd;
		Config 			set_;
		st_				ret;
		bool			loc;
		int				location;
		DirectoryInfo	inf;
		std::map < st_, st_ > text_types;
		std::map < int, st_ > error_codes;
		Server srv;
	public :
		fd_				setting;
		size_t			status_code;
		Response(void);
		bool		headersent;
		bool		sending;
		void		init_TheCont_();
		st_			Create_DefPage();
		Response	&RetResponse( request &req );
		void		isItinConfigFile( st_ URI );
		int			checkMethods( request &req );
		void 		getPage( request &req );
		void		Set_Up_Headers( st_ &ret, request &req, st_ body );
		bool		index_file( request &req, st_ path );
		void		GETResource( request &req );
		void		DELResource( request &req );
		void		DeleteContent( request &req, st_ path );
		int			Fill_Resp( request &req, st_ root );
		void		deleteFile( void );
		void		deleteDir( request &req );
		void		is_file( st_ path, request &req );
		void		is_dir( st_ root, request &req );
		void		setRet( st_ &ret );
		void		content_types();
		void		openDir( st_ path, request &req );
		st_			&getRet();
		int			getFd();
		struct fd_	&getStruct();
		bool		getBool();
		void		clear_Obj();
		void		countCgiBody( request req );
		~Response(void);
};