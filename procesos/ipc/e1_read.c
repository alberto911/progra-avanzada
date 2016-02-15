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
    int fd;
    
    fd = open("/home/alberto911_/fifo", O_RDONLY);
    
    int n;
    while (read(fd, &n, sizeof(int))) {
		if (n != 0)
        	printf("%d! = %d\n", n, factorial(n));
    }
    
    close(fd);
    
    return 0;
}
