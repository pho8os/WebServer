// #include "ConfigFile/ConfigFile.hpp"
#include "ConfigFile/ConfigFile.hpp"
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

int main()
{
	Config a;
	(void)a;
	a.print_config();
}