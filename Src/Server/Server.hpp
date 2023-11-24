#pragma once

#include "../ConfigFile/ConfigFile.hpp"
#include <sys/_types/_size_t.h>
#include <arpa/inet.h>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAX_CLENTS 10

#define PAGE 4096
class MServer
{
	private:
		std::vector<Server> servers;
		size_t nserv;
		std::vector<struct pollfd> fds;
		std::vector<int> servfd;
		std::map<int, std::string> reqs;
		struct addrinfo *addrserv;
	public:
		MServer();
		~MServer();
		void Serving();
		void run();
		void receiving(const size_t &index);
		bool port_exist(size_t &index) const;
};
