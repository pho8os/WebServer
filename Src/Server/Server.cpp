

#include "Server.hpp"
#include <netinet/in.h>
#include <stdexcept>
#include <sys/_types/_size_t.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>


void MServer::run()
{
	unsigned int nserv = this->conf.getConfig().size();
	sockaddr_in addrserv[nserv];
	std::vector<int> servfd[nserv];
	for(size_t i = 0; i < nserv; i++)
	{
		servfd[i].push_back(socket(AF_INET, SOCK_STREAM, 0));
		addrserv[i].sin_family = AF_INET;
		addrserv[i].sin_addr.s_addr = INADDR_ANY;
		addrserv[i].sin_port = htons(this->conf.getConfig()[i].listen.second);
		if(bind(servfd[i][0] , (struct sockaddr *)(&addrserv[i]), sizeof(addrserv[i])) == -1)
			throw(std::runtime_error("Server: Socket failed to bind"));
		if(listen(servfd[i][0], MAX_CLENTS) == -1)
			throw(std::runtime_error("Server: Listening failed"));
	}	
	struct pollfd fds[nserv * (1 + MAX_CLENTS)];
	bzero(fds, sizeof(fds));
	for(size_t i = 0; i < nserv; i++)
	{	
		fds[i].fd = servfd[i][0];
		fds[i].events = POLLIN;
	}
	while(true)
	{
		
	}


	

	
}