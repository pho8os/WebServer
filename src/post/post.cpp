/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 12:49:02 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/16 13:16:53 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "post.hpp"

post::post()
{
}

post::post(str request, str boundary)
: _request(request), _boundary(boundary)
{
    
}

post::~post()
{
}

bool post::isValidReq()
{
}