#include <cstddef>
#include "ConfigFile/ConfigFile.hpp"
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_uintptr_t.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>




int main(int ac, char **av)
{
	Config::setConfig(parseconf(av[1]));
	if(ac != 2)
		return(std::cerr << "Invalid arguments" << std::endl, -1);

}