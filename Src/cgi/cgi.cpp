/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:22:49 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/22 15:56:53 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include "cgi.hpp"

Cgi::Cgi()
{
	this->_php_path = "/usr/bin/php";
	this->_php_cgi = "cgi/cgi.php";
}
Cgi::~Cgi()
{
}
Cgi::Cgi(const Cgi &obj)
{
	(void)obj;
}
Cgi &Cgi::operator=(const Cgi &obj)
{
	(void)obj;
	return *this;
}
bool Cgi::found_ressource(std::string path)
{
	std::cout << path << std::endl;
	if (access(path.c_str(), F_OK) != -1)
	{
		if (access(path.c_str(), F_OK) == 0)
			return true;
	}
	else
		std::cerr << "Error : " << errno << std::endl;
	return false;
}

bool Cgi::execute(std::string query)
{
	(void)query;
	return true;
}
std::string Cgi::get_ress_path(std::string query)
{
	size_t last = query.find_last_of('/');
	if (last != std::string::npos)
	{
		std::string _name = query.substr(last + 1);
		// std::cout << "Resource name: " << _name << std::endl;
		return _name;
	}
	else
		std::cerr << "No path found in the URL" << std::endl;
	return NULL;
}
// accepts a url
bool Cgi::makeResponse(const std::string query)
{

	if (found_ressource(query))
	{
		// part 01 : identify the ressource and
		this->_php_src = get_ress_path(query);
		this->_php_src = query;
		std::string res = getSrcResult();
	}
	else
	{
		std::cerr << "Error : Ressource Not Found !!" << std::endl;
		return false;
	}
	return false;
}

std::string Cgi::getSrcResult()
{
	std::string res("NULL");
	char *envp[] = {NULL};
	const char *tmpFileName = "temp_out";

	int fd = open(tmpFileName, O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
	{
		std::cerr << "Error :Failed to create the tmp_fd !!" << std::endl;
		return res;
	}

	int o_out = dup(STDOUT_FILENO);
	if (o_out == -1)
	{
		std::cerr << "Error :Failed to create the tmp_fd !!" << std::endl;
		close(fd);
		return res;
	}

	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		std::cerr << "Error :Failed to redirect the stdin to tmp_fd !!" << std::endl;
		close(fd);
		return res;
	}

	pid_t a = fork();
	// part 02 : pass the ressource as an argument to the cgi
	if (a == -1)
	{
		std::cerr << "Fork Failed !!" << std::endl;
		return res;
	}
	if (a == 0)
	{
		char *args[] = {(char *)_php_path.c_str(), (char *)_php_cgi.c_str(), (char *)_php_src.c_str(), NULL};
		
		if (execve(_php_path.c_str(), args, envp) == -1)
		{
			perror("execve");
			return res;
		}
	}
	else
	{
		close(fd);
		dup2(o_out, STDOUT_FILENO);
		close (o_out);
		int stat;
		waitpid(a, &stat, 0);
	}
	return res;
}