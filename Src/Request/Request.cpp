/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnassi <mnassi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 11:11:17 by mnassi            #+#    #+#             */
/*   Updated: 2023/12/09 16:47:22 by mnassi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <sys/_types/_size_t.h>
#include <sys/fcntl.h>
#include <unistd.h>

void request::isItinConfigFile( st_ URI, std::vector < Server > server ) {
	int root = -1;
	std::vector < std::string > prefix;
	std::vector <Location> locations = server[0].location;
	for (int idx = 0; idx < (int)locations.size(); idx++)
		prefix.push_back(locations[idx].prefix);
	std::sort(prefix.begin(), prefix.end());
	for (int idx = prefix.size() - 1; idx >= 0; idx--) {
		if (prefix[idx] == "/")
			root = idx;
		else if (prefix[idx] + "/" == URI.substr(0, prefix[idx].length() + 1)) {
			std::cout << prefix[idx] << std::endl;
			for (int i = 0; i < (int)locations.size(); i++)
				if (locations[i].prefix == prefix[idx])
					locate = i;
			return ;
		}
	}
	if (root != -1)
		locate = root;
	else
		throw 404;
}

request::request( st_ request ) : Parsed(true) {
	try {
		upPath = "/goinfre/zmakhkha/up/";
		firstParse = false;
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
			if ((!it_->first.compare("Content-Length") && atoi(it_->second.c_str()) <= 0 && !getMethod_().compare("POST")))
				 throw 400;
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
	firstParse = true;
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
// void	request::printVec(void) {
// 	std::cout << "Method : " << getMethod_() << " URI : " << getURI() << " V : " << getVersion() << " Body : " << getBody() << std::endl;
// 	std::cout << "->> Boundary = " << boundary << std::endl;
// 	for (Map::iterator it_ = headers.begin(); it_ != headers.end(); it_++)
// 		std::cout << it_->first << " ->> " << it_->second << std::endl;
// }

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

std::string	&request::getVersion( void ) {
	return HTTPVersion_;
}
std::string	&request::getURI( void ) {
	return UniformRI;
}
std::string	&request::getMethod_( void ) {
	return Method_;
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

void request::parseboundary(std::string chunk)
{

std::string line = chunk.substr(0, chunk.find("\r\n"));
	chunk.erase(0, line.length() + 2);
	// std::cout << line << std::endl <<  boundary + "--" << std::endl;
	if (line ==  boundary + "--")
	{
		
		reading = 0;
		return ;
	}
	size_t pos = chunk.find("filename=\"");
	if(pos != std::string::npos)
	{	
		std::string file = chunk.substr(pos + 10, (chunk.find("\r\n") - pos - 11));
		// std::cout << "----->" << file << std::endl;
		if(fd > 0)
			close(fd);
		std::string filename = upPath + file;
		// std::cout << "[parseboundary] " << file << std::endl;
		fd = open(filename.c_str(), O_CREAT | O_RDWR, 0644);
	}

}

void request::parsechunk(std::string &chunk)
{
	if(chunk.find( boundary) != std::string::npos && !cgi)
		parseboundary(chunk);
	else
	{
		write(fd, chunk.c_str(), chunk.length());
	}
}

void  request::parseheaders(std::string &page)
{
	size_t pos = page.find("\r\n\r\n");
	if(pos == std::string::npos)
		std::cout << "bad request" << std::endl;
	std::string headers = page.substr(0 , pos + 2);
	page.erase(page.begin(), page.begin() + pos + 4);
}




void request::parseChunked(std::string &page)
{
	static bool a;
	if(!a)
		parseheaders(page);
	while (contentlen < page.length())
	
	{
		if(!contentlen)
		{
			std::string line = page.substr(0, page.find("\r\n"));
			page.erase(page.begin(), page.begin() + line.size() + 2);
			contentlen = hextodec(line);
			if(!contentlen && page[0] == '\r' && page[1] == '\n')
			{
				return (reading = 0, void(0));
			}
		}
		if(contentlen < page.length() )
		{
			chunk += page.substr(0, contentlen);
			page.erase(page.begin(), page.begin() + contentlen + 2);
			contentlen = 0;
			parsechunk(chunk);
			chunk = "";
		}
	}
	if(contentlen >= page.length()) {
		chunk += page;
		contentlen -= page.length();
	}
	a = true;
}

bool request::validboundary(std::string tmp)
{
		size_t pos = tmp.find( boundary);
	if (pos == std::string::npos)
		return false;

	std::string chunk = tmp.substr(0, pos);
	tmp.erase(0, pos);
	
	pos = tmp.find("\r\n\r\n");
	if(pos == std::string::npos && tmp.substr(0,  boundary.length() + 2) !=  boundary + "--")
		return false;
	write(fd, chunk.c_str(), chunk.length() - 4);

	std::string bounds = tmp.substr(0, pos);
	parseboundary(bounds);
	std::string chunk2 = tmp.substr(pos + 4, tmp.length());
	page1 = chunk2;
	pos = page1.find( boundary);
	page2 = "";
	return true;
}

void request::parseSimpleBoundary(std::string &page)
{
	static bool a;
	if(!a)
		parseheaders(page);
	a = true;
	if(page1.empty())
		return (page1 = page, (void)0);
	if(page2.empty())
		page2 = page;
	if(!validboundary(page1 + page2))
	{
		write(fd, page1.c_str(), page1.length());
		std::swap(page1, page2);
		page2 = "";
	}
}

void request::parseMe(st_ request)
{
	try {
		firstParse = true;
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
        //      boundary = boundary;
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

request::request(void){
		upPath = "/goinfre/zmakhkha/up/";
		cgi = false;
		cgiReady = false;
		Parsed = true;
		reading = true;
		firstParse = false;
		cgiBodyPath = "/tmp/";
		contentlen = 0;
}

bool request::getReadStat(void) const
{
	return this->reading;
}

void request::fillCgiBody(const st_ &data)
{
	st_ page = data;
	static bool a;
	if(!a)
		parseheaders(page);
	st_ cgi = cgiBodyPath + "cgiBody";
	while((access(cgi.c_str(), F_OK)))
		cgi += "_";
	int fd = open(cgi.c_str(), O_CREAT | O_RDWR, 0644);
	if (fd < 0)
		perror(st_(st_("Could not create : ") + cgi.c_str()).c_str());
	write(fd, data.c_str(), data.length());
}

void request::feedMe(const st_ &data)
{
	st_ str = data;
	if (firstParse == false)
		parseMe(data);
	if (getMethod_() == "GET" || getMethod_() == "DELETE")
		return (reading = false, void(0));
	if (cgi)
	{
		if (getMethod_() == "POST" && !cgiReady)
			fillCgiBody(str); //keep appending the request untiil fullfilled
		else
		{
			// excecCgi();
		}
	}
	if (getMethod_() == "POST")
	{
		try {
			isItinConfigFile(getURI(), get_.getConfig());
			if (get_.getConfig()[0].location[locate].allow.Post == 0) throw 405;
		}
		catch (int code_) {
			code = code_;
			reading = 0;
			Parsed = false;
			return ;
		}
		(headers["Transfer-Encoding"] == "chunked")
		? parseChunked(str)
		: parseSimpleBoundary(str);
	}
}