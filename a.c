#include <stddef.h>
#include<stdio.h>
#include<limits.h>

int main()
{
	int i = 1;
	size_t j = 9223372036854775807;
	if(i == j)
		printf("hi\n");
	//size_t a = 9223372036854775807 + 1;
	//unsigned long long b = -1;
}