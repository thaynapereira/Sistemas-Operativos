#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>

const char* nome_socket = "socket";
int socket_fd;
int cliente_socket_fd;

char* response(char* input)
{
	time_t now = time(NULL);
   	struct tm *t = localtime(&now);
   	char *str;
   	str = (char*)malloc(100*sizeof(char));
	if(strcmp(input,"Bom dia")==0)
	{
		return "Bom dia";
	}

	else if(strcmp(input,"Iscac")==0)
	{
		return "www.iscac.pt";
	}

	else if(strcmp(input,"data")==0 || strcmp(input,"hoje")==0)
	{
		sprintf(str,"%d/%d/%d %d:%d", t->tm_mday,t->tm_mon+1,t->tm_year+1900,t->tm_hour,t->tm_min);
		return str;
	}

	else if(strcmp(input,"Open Source")==0)
	{
		return "Código Aberto é um termo que indica que um produto inclui permissão para utilizar o seu código-fonte, os seus documentos de desenho (modelos), ou os seus conteúdos";
	}

	else if(strcmp(input,"SO")==0)
	{
		return "Sistemas Operativos";
	}

	else if(strcmp(input,"IG")==0)
	{
		return "Informatica de Gestao";
	}

	else
	{
		return "O servidor nao consegue responder!";
	}
	return "";
}

void escrever_socket(int cliente_socket, const char* resposta)
{
	int tamanho = strlen(resposta) + 1;
	write(cliente_socket, &tamanho, sizeof(tamanho));
	write(cliente_socket, resposta, tamanho);
}

int ler_socket(int cliente_socket)
{
	while(1)
	{
		int tamanho;
		if(read(cliente_socket, &tamanho, sizeof(tamanho)) == 0)
		{
			return 0;
		}
	char* aux = (char*)malloc(100);
	char* mensagem = (char*)malloc(tamanho);
	read(cliente_socket, mensagem, tamanho);
	printf("Recebi do cliente a mensagem : %s\n", mensagem);
	aux = response(mensagem);
	escrever_socket(cliente_socket, aux);
	if(!strcmp(mensagem, "quit"))
	{
		free(mensagem);
		return 1;
	}
	free(mensagem);
    }
}

void terminar(int signum)
{
	
	//terminar socket
	close( socket_fd );
	unlink( nome_socket );

	printf("O servidor terminou!\n");

	exit(0);
}

int main (int argc, char* argv[])
{
	/* criar socket */
	socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);

	struct sockaddr_un nome;

	/* preencher dados para o socket */
	nome.sun_family = AF_LOCAL;
	strcpy(nome.sun_path, nome_socket);

	int mensagem_quit;

	/* fazer a ligação entre dados e socket */
	bind(socket_fd, (struct sockaddr*) &nome, SUN_LEN(&nome));
	/* colocar o socket disponivel para ligações */
	listen(socket_fd,5);

	do
	{
		struct sockaddr_un nome_cliente;
		socklen_t cliente_nome_len;
		cliente_socket_fd = accept(socket_fd, (struct sockaddr*) &nome_cliente, &cliente_nome_len);
		mensagem_quit = ler_socket(cliente_socket_fd);
		close(cliente_socket_fd);
	}
	while(!mensagem_quit);

	close(socket_fd);
	unlink(nome_socket);
	return 0;
}

