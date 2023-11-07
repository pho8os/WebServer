//#include "ConfigFile/ConfigFile.hpp"
#include<iostream>

int main()
{
	std::string obj("widad:zawamil");
	std::cout << obj.substr(obj.find(":"), obj.size() - obj.find(":"));
}