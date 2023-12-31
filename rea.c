#include <sys/fcntl.h>
#include <unistd.h>


int main() {
    int fd = open("./hell.txt", O_RDWR, 0777);
    
}