#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_in direccion;
    
    int servidor, cliente;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
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
    
    cliente = accept(servidor, (struct sockaddr *) &direccion, &tamano);
    
    if (cliente >= 0) {
        printf("Aceptando conexiones en %s:%d \n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));
        
        int tiempo, num, i, total, min, max;
        while (read(cliente, &tiempo, sizeof(int))) {
			total = 0;
            for (i = 0; i < tiempo; ++i) {
				read(cliente, &num, sizeof(int));
				if (i == 0)
					max = min = num;
				else {
					if (num > max)
						max = num;
					if (num < min)
						min = num;
				}
				total += num;
			}

			printf("Mínimo: %d\n", min);
			printf("Máximo: %d\n", max);
			printf("Promedio: %f\n", (total + 0.0) / tiempo);
        }     
    }
    
    // Cerrar el socket
    
    close(servidor);
    close(cliente);
    return 0;
}
