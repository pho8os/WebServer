/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 19:09:44 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/26 21:47:08 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "Response.hpp"

Response::Response()
{
}
Response::Response(const Response &obj)
{
	this->content = obj.getContent();
	this->contentType = obj.getContentType();
	this->statuCode = obj.getStatus();
}
Response::Response(int status, std::string ct, std::string ctt)
	:statuCode(status), content(ct), contentType(ctt)
{
}

Response &Response::operator=(const Response &obj)
{
	if (this != &obj)
	{
		this->content = obj.getContent();
		this->contentType = obj.getContentType();
		this->statuCode = obj.getStatus();
	}
	return *this;
}
Response::~Response()
{
}

void Response::setStatus(const int status)
{
	this->statuCode = status;
}
void Response::setContent(const std::string ct_str)
{
	this->content = ct_str;
}
void Response::setContentType(const std::string ctt_str)
{
	this->contentType = ctt_str;
}

int Response::getStatus(void) const
{
	return this->statuCode;
}
std::string Response::getContent(void) const
{
	return this->content;
}
std::string Response::getContentType(void) const
{
	return this->contentType;
}