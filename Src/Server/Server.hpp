#pragma once

#include "../ConfigFile/ConfigFile.hpp"
#include "../Response/Response.hpp"

#include <deque>
#include <arpa/inet.h>
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/types.h>
#include <netdb.h>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>
#include <ostream>

#define MAX_CLENTS 10
#define PAGE 4096

class request;
class Response;

class MServer
{
	private:
		const std::vector<Server> servers;
		size_t nserv;
		std::vector<struct pollfd> fds;
		std::vector<int> servfd;
		std::map<int , std::pair<request, Response> > clients;
		std::map<int , request> Reqs;
		std::map<int , Response> Resp;


	public:
		MServer();
		~MServer();
		void Serving();
		void run();
		void logerror(const size_t &index, std::string cmd);
		void receiving(const size_t &index);
		void sending(const size_t &index);
		bool port_exist(size_t &index) const;
};
