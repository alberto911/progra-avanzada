#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char** argv) { 
	int n = 1, p = 1, c;

	while ((c = getopt(argc, argv, "n:p:")) != -1) {
		switch (c) {
			case 'n':
				n = atoi(optarg);
				break;
			case 'p':
				p = atoi(optarg);
				break;
			default:
				abort();
		}
	}

	printf("n = %d, p = %d\n", n, p);
	
    return 0;
}
