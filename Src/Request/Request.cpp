/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:11:17 by mnassi            #+#    #+#             */
/*   Updated: 2023/12/06 01:49:09 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <cstdlib>
#include <stdexcept>
#include <sys/_types/_size_t.h>

request::request( st_ request ) : Parsed(true) {
	try {
		A.upPath = "/goinfre/zmakhkha/up/";
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
		if ((pos = headers["Content-Type"].find("boundary=")) != std::string::npos)
        {
			boundary = headers["Content-Type"].substr(pos + 9);
            A.boundary = boundary;
        }
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
			request_.erase(0, found_it + 2);
			size_t found_end = request_.find("\r\n");
			if (found_end == std::string::npos || key.empty())
				break ;
			st_ value = trimString(request_.substr(0, found_end));
			request_.erase(0, found_end + 2);
			headers[key] = value;
		}
		else
			throw 404;
	}
	size_t p = headers["Content-Type"].find("boundary=");
	if (p != st_::npos)
		boundary = headers["Content-Type"].substr(p + 9);
	// if ( !CheckForBody( request_ ) ) return Parsed = false, false;
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
		A.upPath = "/goinfre/zmakhkha/up/";
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

int	request::hextodec(const std::string &s)
{
	int	ret;
	int	hex;


	ret = 0;
	hex = 1;
	for(int i = (int)s.size() - 1; i >= 0; i--)
	{
		if (std::isdigit(s[i]))
			ret += (s[i] - 48) * hex;
		else if (s[i] >= 'A' && s[i] <= 'F')
			ret += (s[i] - 55) * hex;
		else if (s[i] >= 'a' && s[i] <= 'f')
			ret += (s[i] - 87) * hex;
		else
		 	return -1;
		hex *= 16;
	}
	// std::cout << ret << std::endl;
	return (ret);
}

void request::execboundary(std::string s, std::string boundary)
{
	if(s.find(boundary) != std::string::npos)
	{
		s = s.substr(s.find(boundary) + boundary.size() + 2, s.size() );
		std::cout << s << std::endl;
	}

}

void request::parseboundary(std::string chunk, Transfer &A)
{

	std::string line = chunk.substr(0, chunk.find("\r\n"));
	chunk.erase(0, line.length() + 2);
	// std::cout << line << std::endl << A.boundary + "--" << std::endl;
	if (line == A.boundary + "--")
	{
		A.reading = 0;
		return ;
	}
	size_t pos = chunk.find("filename=\"");
	if(pos != std::string::npos)
	{	
		std::string file = chunk.substr(pos + 10, (chunk.find("\r\n") - pos - 11));
		// std::cout << file << std::endl;
		if(A.fd > 0)
			close(A.fd);
		std::string filename = A.upPath + file;
		A.fd = open(filename.c_str(), O_CREAT | O_RDWR, 0644);
	}

}

void request::parsechunk(std::string &chunk,Transfer &A)
{
	if(chunk.find(A.boundary) != std::string::npos && !A.cgi)
		parseboundary(chunk, A);
	else
		write(A.fd, chunk.c_str(), chunk.length());
}

void  request::parseheaders(std::string &page, Transfer &A)
{
	(void)A;
	size_t pos = page.find("\r\n\r\n");
	if(pos == std::string::npos)
		std::cout << "bad request" << std::endl;
	std::string headers = page.substr(0 , pos + 2);
	page.erase(page.begin(), page.begin() + pos + 4);
}




void request::parsepage(std::string &page, Transfer &A)
{
	static bool a;
	if(!a)
		parseheaders(page, A);
	while (A.contentlen < page.length())
	
	{
		if(!A.contentlen)
		{
			std::string line = page.substr(0, page.find("\r\n"));
			A.contentlen = hextodec(line);
			if(!A.contentlen)
				return (A.reading = 0, exit(0));
			page.erase(page.begin(), page.begin() + line.size() + 2);
		}
		if(A.contentlen < page.length() )
		{
			A.chunk += page.substr(0, A.contentlen);
			page.erase(page.begin(), page.begin() + A.contentlen + 2);
			A.contentlen = 0;
			parsechunk(A.chunk, A);
			A.chunk = "";
		}
	}
	if(A.contentlen >= page.length()) {
		A.chunk += page;
		A.contentlen -= page.length();
	}
	a = true;
}

bool request::validboundary(std::string tmp, Transfer &A)
{
	size_t pos = tmp.find(A.boundary);
	if (pos == std::string::npos)
		return true;

	std::string chunk = tmp.substr(0, pos);
	tmp.erase(0, pos);
	
	pos = tmp.find("\r\n\r\n");
	if(pos == std::string::npos && tmp.substr(0, A.boundary.length() + 2) != A.boundary + "--")
		return false;
	write(A.fd, chunk.c_str(), chunk.length() - 2);
	std::string bounds = tmp.substr(0, pos);
	parseboundary(bounds, A);
	// std::cout << bounds << std::endl;
	// exit(0);
	std::string chunk2 = tmp.substr(pos + 4, tmp.length());
	A.page1 = chunk2;
	pos = A.page1.find(A.boundary);
	A.page2 = "";
	return true;
}

void request::parse2pages(std::string &page, Transfer &A)
{
	static int a;
	std::cout << "Entring for the : " << a <<std::endl;
	if(!a)
		parseheaders(page, A);
	a = true;
	if(A.page1.empty())
	{
		if (page.find(A.boundary + "--") == st_::npos)
		return (A.page1 = page, (void)0);			
	}
	if(A.page2.empty())
		A.page2 = page;
	if(!validboundary(A.page1 + A.page2, A))
	{
		write(A.fd, A.page1.c_str(), A.page1.length());
		std::swap(A.page1, A.page2);
		A.page2 = "";
	}
	a++;
}

void request::parseMe(st_ request)
{
	try {
		// size_t	pos = 0;
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
		// if ( headers["Content-Type"].find("boundary=") != std::string::npos && getMethod_() == "POST")
        // {
		// 	boundary = headers["Content-Type"].substr(pos + 9);
        //     A.boundary = boundary;
        // }
		// else {
		// 	std::cout << "Tcha ra laaaaaaa yumkin" << std::endl;
		// }
		KeepAlive = headers["Connection"] == "keep-alive";
	}
	catch(int code_) {
		code = code_;
		Parsed = false;
	}
}
