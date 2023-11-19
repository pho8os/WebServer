#pragma once

#include "../ConfigFile/ConfigFile.hpp"
#define MAX_CLENTS 10
class MServer
{
	private:
		Config conf;
	public:
		void run();
};
