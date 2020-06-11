#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>

void escrever_socket( int socket, const char* mensagem )
{
	int tamanho = strlen( mensagem ) + 1;
	write( socket, &tamanho, sizeof( tamanho ));
	write( socket, mensagem, tamanho);
}

int ler_socket(int socket) 
{

	while(1)
	{
		int tamanho;

		if (read(socket, &tamanho, sizeof(tamanho)) == 0) 
			return 0;

		char* mensagem = (char*)malloc(tamanho);

		read(socket, mensagem, tamanho);

		printf("Resposta do servidor : %s\n", mensagem);

		if(strcmp(mensagem, "O servidor terminou!") == 0) 
		{
			exit(0);
		}

		free(mensagem);
	}

}

void* ler_servidor(void * ptr)
{
	int socket = *(int*)ptr;

	ler_socket(socket);
}

int main()
{
	
	const char* nome_socket = "socket";
	int socket_fd;
	struct sockaddr_un nome;
	pthread_t thread_id;

	/* criar socket */
	socket_fd = socket ( PF_LOCAL, SOCK_STREAM, 0); 

	/* preencher dados para o socket */
	nome.sun_family = AF_LOCAL;
	strcpy( nome.sun_path, nome_socket);

	/* fazer a ligação entre dados e socket */
	connect( socket_fd, (struct sockaddr*)&nome, SUN_LEN(&nome));

	pthread_create( &thread_id, NULL, ler_servidor, &socket_fd);

	char mensagem[256];
	while(1)
	{
		gets(mensagem);
		if(strcmp(mensagem,"start")==0)
		{
			gets(mensagem);
			while(strcmp(mensagem,"end")!=0)
			{
				escrever_socket( socket_fd, mensagem);
				if(strcmp(mensagem,"quit")==0)
				{
					exit(0);
				}
				gets(mensagem);
			}
		}
		else
		{
			printf("Inicie sessão com start\n");
		}
	}
	return 0;
}
