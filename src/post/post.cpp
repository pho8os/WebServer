/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 12:49:02 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/18 12:13:07 by zmakhkha         ###   ########.fr       */
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

void post::SplitReq(str req)
{
    size_t idx = req.find("\r\n\r\n");
    if (idx != std::string::npos)
    {
        this->_headers = req.substr(0, idx);
        this->_body = req.substr(idx + 4, req.length());
    }
    std::cout << this->_headers;
    std::cout << this->_body;
}

std::vector<str> post::ft_split(const str req, const str del)
{
    std::vector<str> res;
    char *tmp;

    tmp = std::strtok((char *)req.c_str(), del.c_str());
    res.push_back(str(tmp));
    while ((tmp = std::strtok(NULL, (char *)del.c_str())))
        res.push_back(str(tmp));
    return res;
}

str post::fileToStr(const str path)
{
    std::ifstream file(path.c_str());
    if (file.is_open())
    {
        str ctn;
        char tmp;
        while (file.get(tmp))
            ctn += tmp;
        file.close();
        return ctn;
    }
    else
        throw std::runtime_error("Could Not Open File !!");
    return NULL;
}

vect<std::map<str, str> > post::fillHeaders(str header)
{
    vect<std::map<str, str> > res;
    vect<str> tmp= ft_split(header, str("\r\n"));
    for(size_t i = 0; i < tmp.size(); i++)
    {
        std::cout << "-------> " << tmp[i] << std::endl;
    }
    return res;
}
