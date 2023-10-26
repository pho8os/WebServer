/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 21:58:44 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/26 22:05:57 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Server.hpp"
#include "Response.hpp"
#include "Cgi.hpp"

int main()
{
	Cgi comm;
	std::cout <<
	comm.found_ressource("src/main.cpp")
	<< std::endl;
}