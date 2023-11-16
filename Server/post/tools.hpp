/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:09:14 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/16 12:13:20 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sstream>


class tools : std::string
{
private:
    /* data */
public:
    tools(/* args */);
    ~tools();

    int calcFields(std::string path, std::string boundary);
};

int tools::calcFields(std::string path, std::string boundary)
{
    
}