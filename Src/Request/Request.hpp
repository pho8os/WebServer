#ifndef REQUEST_HPP
#define REQUEST_HPP

#define BOLD_BLACK "\033[1;30m"
#define BOLD_RED "\033[1;31m"
#define BOLD_GREEN "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE "\033[1;34m"
#define BOLD_PURPLE "\033[1;35m"
#define BOLD_CYAN "\033[1;36m"
#define BOLD_WHITE "\033[1;37m"
#define DEF "\033[0m"

#include "../ConfigFile/ConfigFile.hpp"
#include "../Cgi/Cgi.hpp"

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <algorithm>
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include <map>
#include <vector>
#include <cstddef>
#include <sys/fcntl.h>

#define  K 1024
#define  M 1000 * 1024
#define  G 1000 * 1000 * 1024

#define Map std::map < st_, st_ >
#define st_ std::string

class Cgi;
class Server;

class request {
	private :
		Config	get_;
		Map headers;
		bool parseCgi;
        bool firstParse;
		bool Parsed;
		bool KeepAlive;
        bool cgiReady;
		int	code;
        int Meth;
        int fd;
        int contentlen;
		st_ cgiBodyPath;
    	st_ upPath;
		st_	Method_;
		st_	UniformRI;
		st_	HTTPVersion_;
		st_ boundary;
        st_ chunk;
        st_ fileData;
        st_ page1; 
        st_ page2;
		bool chunkedHeaders;
		bool isChunked;
		int chunklen;
	public :
		Server Serv;
        bool cgi;
		int	locate;
		st_ cgiResult;
        bool reading;
		request();
		request( st_ request );
		void		setMethod_( std::string Method_ );
		void		setURI( std::string URI );
		void		setVersion( std::string version );
		size_t		getCode( void );
		st_			getBoundary( void );
		bool		getBoolean( void );
		const Map	&getVector( void );
		std::string	&getVersion( void );
		std::string	&getURI( void );
		std::string	&getMethod_( void );
		bool		getConnection( void );
		void		printVec(void);
		bool		FillHeaders_( st_ request_ );
		int			CheckForBody();
        void		feed();
		bool		checkURI( st_ URI );
		void 		isItinConfigFile( st_ URI );
		~request( void );

        int		hextodec(const std::string &s);
        void	execboundary(std::string s, std::string boundary);
        void	parseboundary(std::string chunk);
        void	parsechunk(std::string &chunk);
        void	parseheaders(std::string &page);
        bool	validboundary(std::string tmp);
        void	parseSimpleBoundary( std::string &page);
        void	parseChunked( std::string &page);
		void	parseMe(st_ request);
        void	feedMe(const st_ &data);
		bool	getReadStat(void) const;
		void	fillCgiBody(const st_ &data);
		void	clear_Obj();
		void	handleCgi(const st_ &data);
		void	countCgiBody();
		void 	fillCgiBodyNb(const st_ &data);
		void	chunkData(std::string &data);
		bool 	maxBody();
		Server getServer();
};	
#endif