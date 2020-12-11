#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "arbitro.h"
#include "utils.h"
#include "cliente.h"

int abre_serverPipe();
int abre_clientPipe(pid_t pidCliente);
int verificaSeJogadorExiste(CLIENT jogador,CLIENT *listaJogadores,int nJogadoresRecebidos);

int main(int argc, char **argv)
{

	VARS variaveis;
	infoArbitro arbitroSettings;

	variaveis.duracaoCampeonato = -1;
	variaveis.tempoEspera = -1;

	//Obtenção das variáveis de ambiente
	if (getenv("GAMEDIR") != NULL)
	{
		variaveis.gamedir = getenv("GAMEDIR");
	}
	else
	{
		printf("\nVariavel de ambiente GAMEDIR nao detetada, voltando para default -> ~/gamedir");
		variaveis.gamedir = GAMEDIR;
	}

	if (getenv("MAXPLAYERS") != NULL)
	{
		if (atoi(getenv("MAXPLAYERS")) > 30)
		{
			printf("\nValor invalido definido na variavel de ambiente MAXPLAYERS, voltando para default -> %d", MAXPLAYERS);
			variaveis.maxplayers = MAXPLAYERS;
		}
		else
		{
			variaveis.maxplayers = atoi(getenv("MAXPLAYERS"));
		}
	}
	else
	{
		printf("\nVariavel de ambiente MAXPLAYERS nao definida, voltando para default -> %d", MAXPLAYERS);
		variaveis.maxplayers = MAXPLAYERS;
	}

	//obtenção da duração do campeonato e tempo de espera

	int indice;

	while ((indice = getopt(argc, argv, "c:t:")) != -1) //usage: ./arbitro -c 60000 -t 10000
	{
		switch (indice)
		{
		case 'c':
			variaveis.duracaoCampeonato = atoi(optarg);
			printf("\nDuracao campeonato recebida: %d", variaveis.duracaoCampeonato);
			break;

		case 't':
			variaveis.tempoEspera = atoi(optarg);
			printf("\nTempo de espera recebido: %d", variaveis.tempoEspera);
			break;
		default:
			printf("\nArgumento %c inválido", indice);
			break;
		}
	}

	//caso não receba o valor na linha de comandos fica com estes valores
	if (variaveis.tempoEspera == -1)
	{
		printf("\nNao foi recebido nenhum valor para tempo de espera, defaulting to %d", TEMPOESPERA);
		variaveis.tempoEspera = TEMPOESPERA;
	}
	if (variaveis.duracaoCampeonato == -1)
	{
		printf("\nNao foi recebido nenhum valor para a duracao do campeonato, defaulting to %d", DURACAOCAMPEONATO);
		variaveis.duracaoCampeonato = DURACAOCAMPEONATO;
	}

	printf("\ngamedir:%s\nmaxplayers:%d\ntempo espera:%d\nduracao campeonato:%d", variaveis.gamedir, variaveis.maxplayers, variaveis.tempoEspera, variaveis.duracaoCampeonato);


	//1ªfase - obter os jogadores (meta 2)

	CLIENT participantes[MAXPLAYERS];
	int participantesRecebidos = 0;


	int serverpipe_fd = abre_serverPipe();
	int clientpipe_fd;

	char resposta[TAM_MAX];
	pedido_t pedido;

	int nbytes_lidos;
	int nbytes_escritos;

	printf("\nVou entrar no loop");
	fflush(stdout);
	while (1)
	{
		fflush(stdout);
		CLIENT newClient;
		nbytes_lidos = read(serverpipe_fd,&pedido,sizeof(pedido_t));
		printf("\n[CLIENTE %d]jogador: %s",pedido.pidsender,pedido.jogador.nome);
		newClient.pidsender = pedido.pidsender;
		newClient.jogador = pedido.jogador;
		if (participantesRecebidos == variaveis.maxplayers)
		{
			strcpy(resposta,"Servidor cheio!");
			printf("\n[SERVER] responding with -> %s",resposta);
			clientpipe_fd = abre_clientPipe(pedido.pidsender);
			nbytes_escritos = write(clientpipe_fd,resposta,sizeof(resposta));
			close(clientpipe_fd);
		}
		
		if(verificaSeJogadorExiste(newClient,participantes,participantesRecebidos)==0){
			participantes[participantesRecebidos] = newClient;
			participantesRecebidos++;
			strcpy(resposta,"Jogador aceite com sucesso!");
			printf("\n[SERVER] responding with -> %s",resposta);
			clientpipe_fd = abre_clientPipe(pedido.pidsender);
			nbytes_escritos = write(clientpipe_fd,resposta,sizeof(resposta));
			close(clientpipe_fd);
		}else
		{
			strcpy(resposta,"Jogador repetido!");
			printf("\n[SERVER] responding with -> %s",resposta);
			clientpipe_fd = abre_clientPipe(pedido.pidsender);
			nbytes_escritos = write(clientpipe_fd,resposta,sizeof(resposta));
			close(clientpipe_fd);
		}
	}
	return EXIT_SUCCESS;
}

int abre_serverPipe(){
	int serverpipe_fd;

	if (access(ARBITRO_FIFO,F_OK) == -1)
		if (mkfifo(ARBITRO_FIFO, S_IRUSR | S_IWUSR)!= 0)
			//nao foi possivel abrir o pipe
			exit(EXIT_FAILURE);

		if ((serverpipe_fd = open(ARBITRO_FIFO,O_RDWR))==-1)
			exit(EXIT_FAILURE);

	return serverpipe_fd;
}

int abre_clientPipe(pid_t pidCliente){
	int clientpipe_fd;
	char pipe[100];

	sprintf(pipe,CLIENT_FIFO,pidCliente);

	clientpipe_fd = open(pipe,O_WRONLY);
	if (clientpipe_fd == -1)
	{
		printf("\n nao consegui abrir o pipe do cliente, passando a frente");
		exit(EXIT_FAILURE);
	}
	return clientpipe_fd;
	

}

int verificaSeJogadorExiste(CLIENT jogador,CLIENT *listaJogadores,int nJogadoresRecebidos){
	for (int i = 0; i < nJogadoresRecebidos; i++)
	{
		if (strcmp(listaJogadores[i].jogador.nome,jogador.jogador.nome) == 0)
		{
			return 1;
		}
		
	}
	return 0;
	
}