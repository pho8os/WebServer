/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:15:16 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/25 16:12:31 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "Request/post/post.hpp"
#include "Server/Server.hpp"

int main( int ac, char **av ) {
    //server part
     try
    {
        Config a;
		if (ac != 2)
			return std::cout << BOLD_RED << "Usage : \n" << DEF << BOLD_GREEN << "\tAdd the config file\n" << DEF << std::endl, 0;
        MServer serv1(std::atoi(av[1]));
        serv1.initServer();
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
}