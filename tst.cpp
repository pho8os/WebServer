#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <unistd.h>
#include <sys/fcntl.h>

int main()
{
	int fd = open("file", O_RDWR | O_CREAT, 0644);
	write(fd, "\0\0R\0\0B\0", 7);
	char *buffer = (char *)malloc(10);
	close(fd);
	fd = open("file", O_RDWR | O_CREAT, 0644);
	read(fd, buffer, 7);
	std::cout << buffer[5] << std::endl;

}