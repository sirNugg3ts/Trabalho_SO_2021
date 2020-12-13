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
	CLIENT jogador;
	jogador.nextClient = NULL;

	

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
		pedido_t pedidoRegisto;
		printf("\nInsira o seu nome:");
		scanf(" %[^\n]", pedidoRegisto.nomeJogador);
		printf("\nNome recebido:%s", pedidoRegisto.nomeJogador);
		fflush(stdin);

		
		pedidoRegisto.pidsender = getpid();
		resposta_t resposta;

		nbytes_escritos = write(serverpipe_fd,&pedidoRegisto,sizeof(pedidoRegisto));
		if (nbytes_escritos == -1)
		{
			printf("\nErro ao escrever no pipe");
		}
		nbytes_lidos = read(clientpipe_fd,&resposta,sizeof(resposta));
		if (nbytes_lidos == -1)
		{
			printf("\nErro ao ler o pipe");
		}else{
		printf("\n[SERVER SAID] %s\n",resposta.resposta);

		if (strstr(resposta.resposta,"Jogador aceite com sucesso!")!= NULL)
		{
			strcpy(jogador.jogador.nome,pedidoRegisto.nomeJogador);
			strcpy(jogador.jogador.jogo,resposta.jogoAtribuido);
			keepGoing = 0;
		}else if(strstr(resposta.resposta,"Servidor cheio!") != 0){
			keepGoing = 0;
		}else{
			keepGoing = 1;
		}
		}
	}
	//a espera de comando agora

	char comandos[20];
			do{
			printf("Digite um comando (#help para lista completa): ");
			scanf("%s",& *comandos);
			if(strcmp(comandos, "#help") == 0){
			printf("\n********************\n");
			printf("        #mygame\n");
			printf("        #quit\n");
			printf("********************\n");
				}
			else if(strcmp(comandos, "#mygame")==0){
		    printf("Jogo atribuido: %s", jogador.jogador.jogo);
				}
			}while(strcmp(comandos, "#quit")!=0);


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