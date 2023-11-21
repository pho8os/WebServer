#pragma once

#include "../ConfigFile/ConfigFile.hpp"
#define MAX_CLENTS 10

#define PAGE 4096
class MServer
{
	private:
		Config conf;
	public:
		void run();
};
