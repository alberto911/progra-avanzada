#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int factorial(int n) {
	int total = 1, i;
	for (i = 2; i <= n; ++i)
		total *= i;
	return total;
}

int main(int argc, const char * argv[])
{
    int fd_read = open("/home/alberto911_/fifo", O_RDONLY);
	int fd_write = open("/home/alberto911_/fifo2", O_WRONLY);
    
    int n, fact;
    while (read(fd_read, &n, sizeof(int))) {
		if (n != 0) {
			fact = factorial(n);
			write(fd_write, &fact, sizeof(int));
        }
    }
    
    close(fd_write);
	close(fd_read);
    
    return 0;
}
