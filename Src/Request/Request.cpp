/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:11:17 by mnassi            #+#    #+#             */
/*   Updated: 2023/11/19 18:32:58 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

request::request( st_ request ) {
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
	try {
		FillHeaders_(request);
		KeepAlive = headers["Connection"] == "keep-alive";
	}
	catch(int code_) {
		code = code_;
		Parsed = false;
		return;
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
			setBody( request_ );
			break ;
		}
	}
	if (it_ == headers.end() && !getMethod_().compare("POST"))
		throw 400;
	return 1;
}
const Map	&request::getVector( void ) {
	return headers;
}
bool	request::FillHeaders_( st_ request_ ) {
	for (int i = 0; request_.substr(0, 2) != "\r\n" && !request_.empty(); i++) {
		size_t found_it = request_.find(": ");
		if (found_it != std::string::npos) {
			st_ key = request_.substr(0, found_it);
			request_.erase(0, found_it + 2);
			size_t found_end = request_.find("\r\n");
			if (found_end == std::string::npos || key.empty())
				break ;
			st_ value = request_.substr(0, found_end);
			request_.erase(0, found_end + 2);
			headers[key] = value;
		}
		else
			return perror("MetaData Error\n"), Parsed = false, false;
	}
	if (!CheckForBody( request_ )) return Parsed = false, false;
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
	std::cout << "length = " << getBody().length() << std::endl;
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