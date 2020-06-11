#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

const char* nome_socket = "socket";
int socket_fd;
pthread_t *threads;
int *cliente_socket_fd;
int i = 0;
int threads_num = 0;

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

void escrever_socket( int socket, const char* mensagem )
{
	int tamanho = strlen( mensagem ) + 1;
	write( socket, &tamanho, sizeof( tamanho ));
	write( socket, mensagem, tamanho);
}

int ler_socket(int socket,int a)
{
	while(1) 
	{
		int pos = 0;
		int tamanho;
		if (read(socket, &tamanho, sizeof(tamanho)) == 0) 
			return 0;

		char* mensagem = (char*)malloc(tamanho);
		char* aux = (char*)malloc(100);

		read(socket, mensagem, tamanho);

		for(int j=0;j<i;j++)
		{
			if(cliente_socket_fd[j]==a)
			{
				pos = j;
			}
		}
		printf("Recebi do cliente %d, a mensagem : %s\n", pos, mensagem);
		aux = response(mensagem);
		escrever_socket(cliente_socket_fd[pos], aux);

		if(!strcmp(mensagem, "quit"))
		{
			threads_num--;
			for (int j = pos; j < i-1; j++)
			{
         		threads[j] = threads[j+1];
         		cliente_socket_fd[j] = cliente_socket_fd[j+1];
			}
			pthread_cancel(threads[i]);
			pthread_join(threads[i],NULL);
         		i--;
			free(mensagem);
			return 1;
		}
		free(mensagem);
	}
}

void* telefonista ( void* ptr )
{
	int temp = i;
	int a = cliente_socket_fd[i];
	i++;
	int socket = *(int*)ptr;
	printf("Cliente %d vai iniciar a conversa\n", temp);
	ler_socket( socket,a);
}

void terminar(int signum) 
{

	int j;

	for(j=0;j<i;j++)
	{
		printf("Avisar cliente %d\n", j);
		escrever_socket(cliente_socket_fd[j], "O servidor terminou!");
	}
	for(j=0;j<i;j++){
		pthread_cancel(threads[i]);
	}
	for(j=0;j<i;j++){
		pthread_join(threads[i],NULL);
	}

	close( socket_fd );
	unlink( nome_socket );

	printf("O servidor terminou!\n");

	exit(0);
}



int main(int argc,char* argv[]) 
{

	pthread_t threadID;

	struct sockaddr_un nome;

	signal ( SIGINT, terminar );

	int aux;

	int MAX = atoi(argv[1]);

	threads = (pthread_t*)malloc(MAX*sizeof(pthread_t));
	cliente_socket_fd = (int*)malloc(MAX*sizeof(int));

	/* criar socket */
	//parametros - dominio, tipo de dados, protocolo
	socket_fd = socket ( PF_LOCAL, SOCK_STREAM, 0); 

	/* preencher dados para o socket */
	nome.sun_family = AF_LOCAL;
	strcpy( nome.sun_path, nome_socket);

	/* fazer a ligação entre dados e socket */
	bind( socket_fd, (struct sockaddr*)&nome, SUN_LEN(&nome));

	/* colocar o socket disponivel para ligações */
	//2º parametro - quantidade de pedidos que podem ficar em espera
	listen( socket_fd, 3);

	do
	{

		struct sockaddr_un nome_cliente;
		socklen_t cliente_nome_len;

		while(threads_num>=MAX)
		{
			sleep(1);
		}

		sleep(1);

		cliente_socket_fd[i] = accept( socket_fd, (struct sockaddr*)&nome_cliente, &cliente_nome_len);
		
		printf("Conectou-se o cliente %d \n", i);

		pthread_create(&threads[i], NULL, telefonista, &cliente_socket_fd[i]);
		threads_num++;


	} while(1);

	close( socket_fd );
	unlink( nome_socket );
	

	return 0;
}
