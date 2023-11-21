

#include "Server.hpp"
#include <map>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/_types/_size_t.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <vector>


void MServer::run()
{
	unsigned int nserv = this->conf.getConfig().size();
	sockaddr_in addrserv[nserv];
	std::vector<int> servfd;
	std::map<int, std::string> reqs;
	for(size_t i = 0; i < nserv; i++)
	{
		servfd.push_back(socket(AF_INET, SOCK_STREAM, 0));
		addrserv[i].sin_family = AF_INET;
		addrserv[i].sin_addr.s_addr = INADDR_ANY;
		addrserv[i].sin_port = htons(this->conf.getConfig()[i].listen.second);
		if(bind(servfd[i] , (struct sockaddr *)(&addrserv[i]), sizeof(addrserv[i])) == -1)
			throw(std::runtime_error("Server: Socket failed to bind"));
		if(listen(servfd[i], MAX_CLENTS) == -1)
			throw(std::runtime_error("Server: Listening failed"));
	}	

	std::vector<struct pollfd> fds;
	for (size_t i = 0; i < nserv; i++)
	{
		struct pollfd a;
		bzero(&a, sizeof(pollfd));
		a.fd = servfd[i];
		a.events = POLLIN;
		fds.push_back(a);
	}
	while(true)
	{
		poll(fds.data(), fds.size(), -1);
		fds.data();
		for (size_t i = 0; i < nserv; i++)
		{
			if(fds[i].revents & POLLIN)
			{	int client = accept(fds[i].fd, NULL, NULL);
				if(client == -1)
					std::cerr << "accept: A client refused to connect" << std::endl;
				struct pollfd a;
				bzero(&a, sizeof(pollfd));
				a.fd = client;
				a.events = POLLIN;
				fds.push_back(a);
				reqs[client] = "";
			}
		}
		for(size_t j = nserv + 1; j < fds.size(); j++)
		{
			if(fds[j].revents & POLLIN)
			{
				char *data = new char[PAGE];
				int ret = recv(fds[j].fd, data, PAGE, 0);


			}
		}
	}

	// struct pollfd fds[nserv * (1 + MAX_CLENTS)];
	// bzero(fds, sizeof(fds));
	// for(size_t i = 0; i < nserv; i++)
	// {	
	// 	fds[i].fd = servfd[i][0];
	// 	fds[i].events = POLLIN;
	// }
	// while(true)
	// {
	// 	for(size_t i = 0; i < nserv; i++)
	// 	{
	// 		if(fds[i].revents & POLLIN)
	// 		{
	// 			int client = accept(fds[i].fd,NULL, NULL);
	// 			if(client != -1)
	// 			{
	// 				if
	// 			}
	// 			else
	// 				std::cerr << "accept: A client refuse to connect" << std::endl;
	// 		}
	// 	}
	// }


	

	
}