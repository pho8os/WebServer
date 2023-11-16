/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 12:48:26 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/11/16 13:15:51 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>

#define vect std::vector
#define str std::string

class post
{
private:
    std::string _request;
    std::string _boundary;
    vect<std::pair<str, str> > text_data;
    post();
    
public:
    post(str request, str boundary);
    ~post();

    // takes a request body and validate it
    bool isValidReq();
    // takes a valid body and return a vector of sent elements
    vect<std::string> getFields();
    // parse input field
    void parseInput();
    // parse binary Data
    void parse binaryData();
    // make binary files
    bool makeFile();
    
};
