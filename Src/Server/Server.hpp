#pragma once

#include "../ConfigFile/ConfigFile.hpp"
#include "../Request/Request.hpp"
#include "../Response/Response.hpp"

#include <deque>
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
#include "Server.hpp"
#include <arpa/inet.h>
#include <cstdio>
#include <map>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <vector>
#include <ostream>

#define MAX_CLENTS 10

#define PAGE 4096
class MServer
{
	private:
		const std::vector<Server> servers;
		size_t nserv;
		std::vector<struct pollfd> fds;
		std::vector<int> servfd;
		std::map<int, std::string> reqs;
		std::map<int, std::pair<request, Response> > tmp_req;
		request tmp;
		Response tmp_resp;
	    std::ostringstream resp;


	public:
		Transfer A;
		MServer();
		~MServer();
		void Serving();
		void run();
		void receiving(const size_t &index);
		bool port_exist(size_t &index) const;
		void handlePost(st_ t_str, size_t size);
		void acceptClient(const size_t &index);
		void handleClient(const size_t &index);

};
