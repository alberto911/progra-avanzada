#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char * argv[])
{
    int fd, sum = 0;
    
    fd = open("/home/alberto911_/fifo", O_RDONLY);
    
    int n;
    while (read(fd, &n, sizeof(int))) {
		if (n != 0)
			sum += n;
		else
        	printf("Suma = %d\n", sum);
    }
    
    close(fd);
    
    return 0;
}
