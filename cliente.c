#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

const char* nome_socket = "socket";

int ler_socket(int cliente_socket)
{
	int tamanho;
	if(read(cliente_socket, &tamanho, sizeof(tamanho)) ==0)
		return 0;

	char* mensagem = (char*)malloc(tamanho);
	read(cliente_socket, mensagem, tamanho);
	printf("Resposta do servidor : %s\n", mensagem);
	
	if(strcmp(mensagem, "O servidor terminou!") == 0) 
	{
		exit(0);
	}
	free(mensagem);
}

void escrever_socket(int socket_fd, const char* mensagem)
{
	int tamanho = strlen(mensagem) + 1;
	write(socket_fd, &tamanho, sizeof(tamanho));
	write(socket_fd, mensagem, tamanho);
}

int main (int argc, char* argv[])
{
	int socket_fd;

	char mensagem[50];

	while(1)
	{
		gets(mensagem);
		if(strcmp(mensagem, "start") == 0)
		{
			/* criar socket */
			socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);

			struct sockaddr_un nome;

			/* preencher dados para o socket */
			nome.sun_family = AF_LOCAL;
			strcpy(nome.sun_path, nome_socket);

			/* fazer a ligação entre dados e socket */
			connect(socket_fd, (struct sockaddr*) &nome, SUN_LEN(&nome));

			gets(mensagem);
			while(strcmp(mensagem, "end") != 0)
			{
				escrever_socket(socket_fd, mensagem);
				ler_socket(socket_fd);
				gets(mensagem);
			}
			close(socket_fd);
		}
		else
		{
			printf("Inicie sessao com start\n");
		}
	}
	
}

	


