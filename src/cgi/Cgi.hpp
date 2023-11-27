/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:17:25 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/27 17:49:16 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <map>
#include <algorithm>
#include <vector>
#include "../Request/Request.cpp"

#define st_ std::string

class Cgi
{
private:
	request req;
  	std::vector<st_> _env;
	st_ _phpPath;
	st_ _pythonPath;

public:
	Cgi();
	Cgi(std::map<st_, st_>);
	Cgi(const Cgi &obj);
	Cgi &operator=(const Cgi &obj);
	~Cgi();

	
	void execute(st_ uri);
	void setEnvVars(std::map<st_, st_> map);
	bool makeResponse(const st_ query);
	st_ getSrcResult();
};