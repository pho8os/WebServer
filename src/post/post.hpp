/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 12:48:26 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/18 12:07:13 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <pages.hpp>

#define vect std::vector
#define str std::string

class post
{
private:
    str _request;
    str _headers;
    str _body;
    str _sign;
    vect<std::pair<str, str> > text_data;

public:
    post();
    post(str request, str boundary);
    ~post();

    // get the header and the body
    void SplitReq(str req);
    // takes a request body and validate it
    bool isValidReq();
    // fill headers
    vect<std::map<str, str> > fillHeaders(str header);
    // takes a valid body and return a vector of sent elements
    vect<str> getFields();
    // parse input field
    void parseInput();
    // parse binary Data
    void parseBinaryData();
    // make binary files
    bool makeFile();

    // tools
    std::vector<str> ft_split(const str req, const str del);
    str fileToStr(const str path);
};
