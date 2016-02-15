#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, const char * argv[])
{
    int error = mkfifo("/home/alberto911_/fifo", 0777);
	int error2 = mkfifo("/home/alberto911_/fifo2", 0777);
    
    if (error || error2) {
        printf("Error al crear el FIFO\n");
        return 1;
    }
    
    int fd_write = open("/home/alberto911_/fifo", O_WRONLY);
	int fd_read = open("/home/alberto911_/fifo2", O_RDONLY);
    
    int n, fact;
	while (1) {
        printf("Introduce un número: ");
		scanf("%d", &n);
        write(fd_write, &n, sizeof(int));
		
		if (n == 0)
			break;

		read(fd_read, &fact, sizeof(int));
		printf("%d! = %d\n", n, fact);
    }
    
    close(fd_write);
	close(fd_read);
    unlink("/home/alberto911_/fifo");
	unlink("/home/alberto911_/fifo2");
    
    return 0;
}
