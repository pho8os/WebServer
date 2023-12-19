#include <cstddef>
#include "ConfigFile/ConfigFile.hpp"
#include "Server/Server.hpp"
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_uintptr_t.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>


//std::string file_gen()
//{
//  static size_t i;
//  return (std::string("/tmp/file") + std::to_string(i++));
//}

int main(int ac, char **av)
{
	if(ac != 2)
		return(std::cerr << "Invalid arguments" << std::endl, -1);
	Config::setConfig(parseconf(av[1]));
	Config a;
	(void)a;
	
	MServer server;
	try{

		server.Serving();
	}
	catch (std::exception &e)
	{
		std::cout << "---------++++\n";
		std::cout << e.what() << std::endl;
	}

}