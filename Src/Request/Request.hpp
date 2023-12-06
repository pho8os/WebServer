/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:10:28 by mnassi            #+#    #+#             */
/*   Updated: 2023/12/05 23:01:09 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <algorithm>
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include <map>
#include <vector>
#include "../ConfigFile/ConfigFile.hpp"

#include <cstddef>
#include <iostream>
#include <map>
#include <fstream>
#include <sys/fcntl.h>

// #include "../cgi/cgi.hpp"
#define Map std::map < st_, st_ >
#define st_ std::string


struct Transfer
{
    std::map<st_ , st_> Headers;
    int Meth;
    bool cgi;
    bool chunked;
    int fd;
    size_t contentlen;
    bool reading;
    st_ chunk;
    st_ boundary;
    st_ fileData;
    st_ page1;
    st_ page2;
	st_ upPath;
    Transfer()
    {
        contentlen = 0;
        boundary = "";
        reading = 1;
        cgi = 0;
        // fd = open("cgi", O_CREAT | O_RDWR, 0644);
    }
};

class request {
	private :
		// Config	get_;
		int	code;
		st_	Method_;
		st_	UniformRI;
		st_	HTTPVersion_;
		st_	body;
		st_ boundary;
		Map headers;
		bool Parsed;
		bool KeepAlive;
	public :
    Transfer A;
		request( void );
		request( st_ request );
		void	setMethod_( std::string Method_ );
		void	setURI( std::string URI );
		void	setVersion( std::string version );
		void	setBody( std::string body );
		size_t		getCode( void );
		st_			getBoundary( void );
		bool		getBoolean( void );
		const Map	&getVector( void );
		std::string	&getBody( void );
		std::string	&getVersion( void );
		std::string	&getURI( void );
		std::string	&getMethod_( void );
		bool		getConnection( void );
		void	printVec(void);
		bool	FillHeaders_( st_ request_ );
		int	CheckForBody( st_ request_ );
		bool	checkURI( st_ URI );
		~request( void );


        int	hextodec(const std::string &s);
        void execboundary(std::string s, std::string boundary);
        void parseboundary(std::string chunk, Transfer &A);
        void parsechunk(std::string &chunk,Transfer &A);
        void  parseheaders(std::string &page, Transfer &A);
        bool validboundary(std::string tmp, Transfer &A);
        void parse2pages( std::string &page, Transfer &A);
        void parsepage( std::string &page, Transfer &A);

		void parseMe(st_ request);


};

#endif