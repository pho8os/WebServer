/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:15:16 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/06 19:59:42 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/Request.hpp"
#include "Response/Response.hpp"

int main( int ac, char **av ) {
	if (ac != 2)
		return std::cout << BOLD_RED << "Usage : \n" << DEF << BOLD_GREEN << "\tAdd the config file\n" << DEF << std::endl, 0;
	(void)av;
	server	init_;
	request	inst_;
	Response inst;
	init_.set_up( inst_ );
	inst = inst.RetResponse( inst_ );
}