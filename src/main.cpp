/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 21:58:44 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/18 11:56:45 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server/Server.hpp"
#include "post/post.hpp"
int main(int ac, char **av)
{
    //  try
    // {
    //     if (ac != 2)
    //         throw std::runtime_error("Bad args for testing");
    //     Server serv1(std::atoi(av[1]));
    //     serv1.initServer();
    // }
    // catch(std::exception &e)
    // {
    //     std::cout << e.what() << '\n';
    // }

    (void)ac;
    (void)av;
    try
    {
        post p;
        std::string st = p.fileToStr("./src/post/request_example");
        p.SplitReq(st);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}