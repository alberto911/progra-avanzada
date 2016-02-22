#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define TCP_PORT 8000

int main(int argc, const char * argv[]) {
    
    struct sockaddr_in direccion;
    
    int cliente;
    
    int leidos, escritos;
    
    if (argc != 2) {
        printf("Error. Use: %s A.B.C.D \n", argv[0]);
        exit(-1);
    }
    
    //Crear el socket
    cliente = socket(PF_INET, SOCK_STREAM, 0);
    
    // Establecer conexión
    direccion.sin_port = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);
    
    int estado = connect(cliente, (struct sockaddr *) &direccion, sizeof(direccion));
    
    // Comunicación
    
    if (estado == 0) {
        printf("Conectado a %s:%d \n", inet_ntoa(direccion.sin_addr), ntohs(direccion.sin_port));
        
        srand((int) time(NULL));
		int *datos, *p, tiempo, continuar = 1;

		while (continuar) {
			continuar = 0;
			tiempo = (rand() % 10) + 1;
			write(cliente, &tiempo, sizeof(int));			
	
			datos = (int*) malloc (tiempo * sizeof(int));

			// Recopilar datos
			for (p = datos; p < datos + tiempo; ++p) {
				*p = rand() % 40;

				if (!continuar && *p != 0)
					continuar = 1;

				printf("%d\n", *p);
				sleep(1);
			}

			for (p = datos; p < datos + tiempo; ++p) {
				write(cliente, p, sizeof(int));
			}
			
			printf("\n");
			free(datos);
		}
    }
    
    // Cerrar el socket
    close(cliente);

    return 0;
}
