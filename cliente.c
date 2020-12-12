#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"
#include "utils.h"
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int abrepipe_servidor();
int abrepipe_cliente();

int main(int argc, char **argv)
{

	

	////////////////////////////

	int serverpipe_fd = abrepipe_servidor();
	int clientpipe_fd = abrepipe_cliente();
	int nbytes_escritos;
	int nbytes_lidos;

	signal(SIGPIPE,SIG_IGN);

	int keepGoing = 1;

	while (keepGoing)
	{	
		setbuf(stdout,NULL);
		//obter identificação do jogador
		PLAYER jogador;
		printf("\nInsira o seu nome:");
		scanf(" %[^\n]", jogador.nome);
		printf("\nNome recebido:%s", jogador.nome);
		fflush(stdin);

		pedido_t pedido;
		pedido.pidsender = getpid();
		pedido.jogador = jogador;
		char resposta[TAM_MAX];

		nbytes_escritos = write(serverpipe_fd,&pedido,sizeof(pedido));
		if (nbytes_escritos == -1)
		{
			printf("\nErro ao escrever no pipe");
		}
		nbytes_lidos = read(clientpipe_fd,resposta,sizeof(resposta));
		if (nbytes_lidos == -1)
		{
			printf("\nErro ao ler o pipe");
		}else{
		printf("\n[SERVER SAID] %s\n",resposta);

		if (strcmp(resposta,"Jogador aceite com sucesso!") == 0)
		{
			keepGoing = 0;
		}else if(strcmp(resposta,"Servidor cheio!")== 0){
			keepGoing = 0;
		}
		}


		
	}
	return EXIT_SUCCESS;
}

int abrepipe_servidor(){
	int serverpipe_gd;

	if (access(ARBITRO_FIFO,F_OK)==-1)
	{
		fprintf(stderr,"\nPipe nao existe, o servidor esta a correr?");
		exit(EXIT_FAILURE);
	}

	if ((serverpipe_gd= open(ARBITRO_FIFO,O_WRONLY))==-1)
	{
		fprintf(stderr,"\nBONK");
		exit(EXIT_FAILURE);
	}

	return serverpipe_gd;
}

int abrepipe_cliente(){
	int clientpipe_fd;
	char clientpipe[50];

	sprintf(clientpipe,CLIENT_FIFO,getpid());

	if (access(clientpipe,F_OK) == -1)
	{
		if (mkfifo(clientpipe, 0666) != 0)
		{
			printf("\nBONK");
			exit(EXIT_FAILURE);
		}
		
	}else
	{
		printf("\nPipe ja existe, yeeeeeeeet");
	}

	if ((clientpipe_fd = open(clientpipe,O_RDWR))==-1)
	{
		exit(EXIT_FAILURE);
	}

	return clientpipe_fd;
}