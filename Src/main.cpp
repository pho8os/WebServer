/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:15:16 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/22 10:02:53 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request/Request.hpp"
#include "Response/Response.hpp"
#include "Request/post/post.hpp"
#include "Server/Server.hpp"

int main(int ac, char **av)
{
    //  try
    // {
    //     if (ac != 2)
    //         throw std::runtime_error("Bad args for testing");
    //     MServer serv1(std::atoi(av[1]));
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
        p.runPost();
        // p.SplitReq(st);
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}