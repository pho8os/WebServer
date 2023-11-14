/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:10:28 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/14 14:59:17 by mnassi           ###   ########.fr       */
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
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include <map>
#include <vector>
#include "../Server/Server.hpp"
#include "../ConfigFile/ConfigFile.hpp"
#define Vect std::vector < std::pair < st_, st_ > >
#define st_ std::string
class request {
	private :
		Config	get_;
		st_	Method_;
		st_	UniformRI;
		st_	HTTPVersion_;
		st_	body;
		st_	buffer;
		bool Parsed;
		Vect headers;
	public :
		request( void );
		void	setMethod_( std::string Method_ );
		void	setURI( std::string URI );
		void	setVersion( std::string version );
		void	setBody( std::string body );
		void	setBuffer( std::string buffer );
		const Vect	&getVector( void );
		std::string	&getBuffer( void );
		std::string	&getBody( void );
		std::string	&getVersion( void );
		std::string	&getURI( void );
		std::string	&getMethod_( void );
		void	printVec(void);
		void	HTTPRequest( void );
		bool	FillHeaders_( st_ request_ );
		bool	FillBody( st_ request_, int error_code );
		int	CheckForBody( st_ request_ );
		bool	checkURI( st_ URI );
		~request( void );
};

#endif