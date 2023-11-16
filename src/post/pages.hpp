/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pages.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 13:37:05 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/16 13:42:44 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <iostream>
class pages
{
private:
    /* data */
public:
    pages(/* args */);
    ~pages();

    class p201 : public std::exception
    {
        const char *what() const throw();
    };
    class p206 : public std::exception
    {
        const char *what() const throw();
    };
    class p304 : public std::exception
    {
        const char *what() const throw();
    };
    class p416 : public std::exception
    {
        const char *what() const throw();
    };
};

pages::pages(/* args */)
{
}

pages::~pages()
{
}
