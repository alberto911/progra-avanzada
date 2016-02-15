#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char * argv[])
{
    int error = mkfifo("/home/alberto911_/fifo", 0777);
    
    if (error) {
        printf("Error al crear el FIFO\n");
        return 1;
    }
    
    int fd = open("/home/alberto911_/fifo", O_WRONLY);
    
    int n;
	do {
        printf("Introduce un número: ");
		scanf("%d", &n);
        write(fd, &n, sizeof(int));
    } while (n != 0);
    
    close(fd);
    unlink("/home/alberto911_/fifo");
    
    return 0;
}
