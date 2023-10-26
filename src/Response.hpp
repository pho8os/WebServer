/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 19:09:47 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/26 21:45:58 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#pragma once
#include <string>
class Response
{
private:
	int statuCode; // 200 400  404 500
	std::string content;
	std::string contentType;

public:
	Response();
	Response(int status, std::string ct, std::string ctt);
	Response(const Response &obj);
	Response &operator=(const Response &obj);
	~Response();

	void setStatus(const int status);
	void setContent(const std::string ct_str);
	void setContentType(const std::string ctt_str);

	int getStatus(void) const;
	std::string getContent(void) const;
	std::string getContentType(void) const;
	
};
