#include "Src/Transfer/Transfer.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <stack>
#include <string>
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
	std::cout << ret << std::endl;
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

void parsechunk(std::string &chunk,Transfer &A)
{
	std::cout << "--" << chunk << "--\n" ;
	// exit(0);
}

void  parseheaders(std::string &page, Transfer &A)
{
	size_t pos = page.find("\r\n\r\n");
	if(pos == std::string::npos)
		std::cout << "bad request" << std::endl;
	std::string headers = page.substr(0 , pos + 2);
	std::cout << headers;
	A.boundary = "----------------------------212426819241453462870583";
	page.erase(page.begin(), page.begin() + pos + 4);
}


void parseboundary(std::string &page, Transfer &A)
{
	std::stack<int> s;
}


void parsepage(std::string page, int ret, Transfer &A)
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
				exit(0);
			page.erase(page.begin(), page.begin() + line.size() + 2);
		}
		if(A.contentlen < page.length())
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

int main()
{



	int fd = open("a", O_RDWR);
	Transfer A;
	A.cgi = false;
	while(!std::cin.eof())
	{
		char *chunk = new char[4096];
		int ret = read(fd, chunk, 4096);
		std::string str(chunk, ret);
		parsepage(str, ret, A);
		delete [] chunk;
		

	}
}


// ----------------------------212426819241453462870583
// Content-Disposition:     form-data; name="zbuba"; filename="file3"
// Content-Type: application/octet-stream