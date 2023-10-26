/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:17:25 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/26 22:11:48 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include<iostream>
#include<string>
#include <unistd.h>
#include <cerrno>

class Cgi
{
	private:
		std::string _type;
	public:
		Cgi();
		~Cgi();
		Cgi(const Cgi &obj);
		Cgi& operator=(const Cgi &obj);
		std::string get_ress_path(std::string query);
		bool found_ressource(std::string path);
		bool execute(std::string query);
		bool makeResponse(const std::string query);
};