/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:22:49 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/26 22:12:39 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Cgi.hpp"

Cgi::Cgi()
{
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
		std::cout << "Resource name: " << _name << std::endl;
		return _name;
	}
	else
		std::cerr << "No path found in the URL" << std::endl;
	return NULL;
}
bool Cgi::makeResponse(const std::string query)
{
	if (found_ressource(query))
	{
		std::string path = get_ress_path(query);
		// needs to fill a response object
	}
	else
	{
		std::cerr << "Error : Ressource Not Found !!" << std::endl;
		return false;
	}
}
