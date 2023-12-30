#include "ConfigFile/ConfigFile.hpp"
#include "Server/Server.hpp"
#include <cstddef>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

st_ getConf(int ac, char **av) {
  st_ conf;
  if (ac == 1) {
    std::cout <<  "Loading default server !!" << std::endl;
    conf = "config/.config";
  } else if (ac == 2) {
    conf = av[1];
  } else{
    std::cerr << "Invalid arguments" << std::endl;
	  conf = "";
  }
  if (access(conf.c_str(), F_OK) != 0)
    throw std::runtime_error("Please provide a valid config file !!");
  return conf;
}

int main(int ac, char **av) {

  try {
  st_ conf = getConf(ac, av);
  Config::setConfig(parseconf(conf));
  Config a;
  (void)a;
  MServer server;
    server.Serving();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}