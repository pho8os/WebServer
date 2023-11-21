/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:05:27 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/19 15:57:01 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

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
#include <sys/types.h>
#include <dirent.h>
#include <netinet/in.h>
#include <fstream>
#include <unistd.h>
#include <map>
#include <vector>
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"
class Response;
class request;

#define st_ std::string

class server {
	public :
		server( void );
		void	set_up( Response &res );
		~server( void );
};

#endif