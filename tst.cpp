:#include "Src/Transfer/Transfer.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <string>
#include <sys/_types/_ssize_t.h>
#include <unistd.h>
#include <sys/fcntl.h>



int	hextodec(const std::string &s)
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

void execboundary(std::string s, std::string boundary)
{
	if(s.find(boundary) != std::string::npos)
	{
		s = s.substr(s.find(boundary) + boundary.size() + 2, s.size() );
		std::cout << s << std::endl;
	}

}

void parseboundary(std::string chunk, Transfer &A)
{

	std::string line = chunk.substr(0, chunk.find("\r\n"));
	chunk.erase(0, line.length() + 2);
	// std::cout << line << std::endl << A.boundary + "--" << std::endl;
	if (line == A.boundary + "--")
	{
		A.reading = 0;
		return ;
	}
	int pos = chunk.find("filename=\"");
	if(pos != std::string::npos)
	{	
		std::string file = chunk.substr(pos + 10, (chunk.find("\r\n") - pos - 11));
		// std::cout << file << std::endl;
		if(A.fd > 0)
			close(A.fd);
		std::string filename = "/goinfre/zmakhkha/up/" + file;
		A.fd = open(filename.c_str(), O_CREAT | O_RDWR, 0644);
	}

}

void parsechunk(std::string &chunk,Transfer &A)
{
	if(chunk.find(A.boundary) != std::string::npos && !A.cgi)
		parseboundary(chunk, A);
	else
		write(A.fd, chunk.c_str(), chunk.length());
}

void  parseheaders(std::string &page, Transfer &A)
{
	size_t pos = page.find("\r\n\r\n");
	if(pos == std::string::npos)
		std::cout << "bad request" << std::endl;
	std::string headers = page.substr(0 , pos + 2);
	page.erase(page.begin(), page.begin() + pos + 4);
}




void parsepage(std::string &page, Transfer &A)
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

bool validboundary(std::string tmp, Transfer &A)
{
	size_t pos = tmp.find(A.boundary);
	if (pos == std::string::npos)
		return false;

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

void parse2pages(std::string &page, Transfer &A)
{
	static bool a;
	if(!a)
		parseheaders(page, A);
	a = true;
	if(A.page1.empty())
		return (A.page1 = page, (void)0);
	if(A.page2.empty())
		A.page2 = page;
	if(!validboundary(A.page1 + A.page2, A))
	{
		write(A.fd, A.page1.c_str(), A.page1.length());
		std::swap(A.page1, A.page2);
		A.page2 = "";
	}
}

int main()
{



	int fd = open("/goinfre/zmakhkha/req_exp", O_RDWR);
	Transfer A;
	// parseboundary("----------------------------119293058853605376629782\r\n", A);
	// exit(0);
	while(A.reading)
	{
		char *chunk = new char[65356];
		int ret = read(fd, chunk, 65356);
		std::string str(chunk, ret);
		// std::cout << str;
		// exit(0);
		// parsepage(str, A);
		parse2pages(str, A);
		delete [] chunk;
	}
}


// ----------------------------212426819241453462870583
// Content-Disposition:     form-data; name="zbuba"; filename="file3"
// Content-Type: application/octet-stream
