#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <sys/fcntl.h>

int main()
{
int a = 10;
int *p = &a;
std::cout << "address p is "<< std::hex << (unsigned long)p << std::endl;
std::cout << "value p is "<< (unsigned long)*p << std::endl;
std::cout << "address p + 1 is "  << std::hex << (unsigned long)(p + 1) << std::endl;
std::cout << "value p + 1 is "<< *(p + 1) << std::endl;

}