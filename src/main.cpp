/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 21:58:44 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/27 23:27:19 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Server.hpp"
#include "Response.hpp"
#include "Cgi.hpp"

int main()
{
	try
	{
		/* code */
		Cgi comm;
		std::cout << comm.makeResponse("cgi/page2.php")
				  << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}