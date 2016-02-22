#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_in direccion;
    char buffer[1000];
    
    int servidor, cliente, pid, fact;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
	//PF_UNIX y AF_UNIX para sockets unix
    servidor = socket(PF_INET, SOCK_STREAM, 0);
    
    // Enlace con el socket
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);
    
    bind(servidor, (struct sockaddr *) &direccion, sizeof(direccion));
    
    // Escuchar a los clientes
    listen(servidor, 10);
    
    // Comunicación
    socklen_t tamano = sizeof(direccion);

	while (1) {
    
		cliente = accept(servidor, (struct sockaddr *) &direccion, &tamano);

		printf("Aceptando conexiones en %s:%d \n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));

		pid = fork();	
		if (pid == 0)
			break;
	}
    
	if (pid == 0) {	
		close(servidor);

		if (cliente >= 0) {
		    // Leer de socket y escribir en pantalla
		    while (leidos = read(cliente, &buffer, sizeof(buffer))) {
		        write(fileno(stdout), &buffer, leidos);
				
				fact = atoi(buffer) * 2;
				printf("Resultado: %d\n", fact);
				sprintf(buffer, "%d", fact);	        

		        //leidos = read(fileno(stdin), &buffer, sizeof(buffer));
		        write(cliente, &buffer, sizeof(int));
		    }     
		}
		
		close(cliente);
	}
	else if (pid > 0) {
		while (wait(NULL) != -1);

		close(servidor);
	}
    
    return 0;
}
