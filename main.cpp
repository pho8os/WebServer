/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:15:16 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/22 22:31:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Src/Server/Server.hpp"

int main( int ac, char **av ) {
    //server part
     try
    {
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