/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:11:17 by mnassi            #+#    #+#             */
/*   Updated: 2023/12/07 14:09:24 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void		LowerHeaders( st_ &header ) {
	for (int i = 0; i < (int)header.length(); i++)
		header[i] = std::tolower(header[i]);
}

request::request( st_ request ) : Parsed(true) {
	try {
		A.reading = 1;
		size_t	pos = 0;
		size_t delete_ = 0;
		for (int i = 0; request[i]; i++) {
			delete_ = request.find(" ");
			if (delete_ != std::string::npos && Method_.empty())
				this->setMethod_(request.substr(0, delete_));
			else if (delete_ != std::string::npos && UniformRI.empty()) {
				if (!checkURI( request.substr(0, delete_) ))
					return ;
				this->setURI(request.substr(0, delete_));
			}
			
			else
				break ;
			request.erase(0, delete_ + 1);
		}
		if (getMethod_() != "POST" && getMethod_() != "GET" && getMethod_() != "DELETE") throw 501;
		delete_ = request.find("\r\n");
		if (delete_ != std::string::npos)
			setVersion(request.substr(0, delete_));
		if (getVersion() != "HTTP/1.1") throw 505;
		request.erase(0, delete_ + 2);
		FillHeaders_(request);
		if ((pos = headers["content_type"].find("boundary=")) != std::string::npos)
			boundary = headers["Content-Type"].substr(pos + 9);
		KeepAlive = headers["Connection"] == "keep-alive";
	}
	catch(int code_) {
		code = code_;
		Parsed = false;
	}
}
int	request::CheckForBody( st_ request_ ) {
	Map::iterator it_ = headers.begin();
	for (; it_ != headers.end(); it_++) {
		if ((!it_->first.compare("Content-Length")) || (!it_->first.compare("Transfer-Encoding"))) {
			if ((!it_->first.compare("Content-Length") && atoi(it_->second.c_str()) <= 0 && !getMethod_().compare("POST")) 
				|| ((int)body.length() > atoi(it_->second.c_str()) && !getMethod_().compare("POST"))) throw 400;
			else if (!it_->first.compare("Transfer-Encoding") && it_->second.compare("chunked")) throw 501;
			request_.erase(0, request_.find("\r\n") + 2);
			break ;
		}
	}
	// if (it_ == headers.end() && !getMethod_().compare("POST"))
	// 	throw 400;
	return 1;
}
const Map	&request::getVector( void ) {
	return headers;
}
st_		trimString( st_ sub ) {
	int i;
	for (i = 0; sub[i] && sub[i] == ' '; i++);
	return &sub[i];
}
bool	request::FillHeaders_( st_ request_ ) {
	for (int i = 0; request_.substr(0, 2) != "\r\n" && !request_.empty(); i++) {
		size_t found_it = request_.find(":");
		if (found_it != std::string::npos) {
			st_ key = trimString(request_.substr(0, found_it));
			LowerHeaders(key);
			request_.erase(0, found_it + 2);
			size_t found_end = request_.find("\r\n");
			if (found_end == std::string::npos || key.empty())
				break ;
			st_ value = trimString(request_.substr(0, found_end));
			LowerHeaders(value);
			request_.erase(0, found_end + 2);
			headers[key] = value;
		}
		else
			throw 404;
	}
	if ( !CheckForBody( request_ ) ) return Parsed = false, false;
	return true;
}
bool	request::checkURI( st_ URI ) {
	if (URI.length() > 2048)
		throw 414;
	st_ Allowed = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	for (int i = 0; URI[i]; i++) {
		int check = 0;
		for (; Allowed[check]; check++)
			if (Allowed[check] == URI[i])
				break ;
		if (Allowed[check] == '\0')
			throw 400;
	}
	return true;
}
void	request::printVec(void) {
	std::cout << "Method : " << getMethod_() << " URI : " << getURI() << " V : " << getVersion() << " Body : " << getBody() << std::endl;
	std::cout << "->> Boundary = " << boundary << std::endl;
	for (Map::iterator it_ = headers.begin(); it_ != headers.end(); it_++)
		std::cout << it_->first << " ->> " << it_->second << std::endl;
}
request::request(void) : Parsed(true) {

}
request::~request(void) {

}
bool		request::getConnection( void ) {
	return KeepAlive;
}
void	request::setMethod_( std::string Method_ ) {
	this->Method_ = Method_;
}
void	request::setURI( std::string URI ) {
	this->UniformRI = URI;
}
void	request::setVersion( std::string version ) {
	this->HTTPVersion_ = version;
}
void	request::setBody( std::string body ) {
	this->body = body;
}
std::string	&request::getVersion( void ) {
	return HTTPVersion_;
}
std::string	&request::getURI( void ) {
	return UniformRI;
}
std::string	&request::getMethod_( void ) {
	return Method_;
}
std::string	&request::getBody( void ) {
	return body;
}
bool		request::getBoolean( void ) {
	return Parsed;
}
size_t		request::getCode( void ) {
	return code;
}
st_			request::getBoundary( void ) {
	return boundary;
}