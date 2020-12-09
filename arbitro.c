#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "arbitro.h"
#include "utils.h"

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

	PLAYER listaJogadores[MAXPLAYERS];
	int nJogadoresRecebidos = 0;

	int res = makefifo(ARBITRO_FIFO, 0777);

	if (res == -1)
	{
		perror("\nErro ao criar fifo");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "\nFIFO servidor criado");

	while(1){ //a substituir por tempo maximo de esperaa na meta 3
		char * buffer;
		int s_fifo_fd = open(ARBITRO_FIFO,O_RDONLY);
		read(s_fifo_fd,buffer,50);
	}

	//2ªfase - jogo (para meta 3)

	int s_fifo_fd = open(ARBITRO_FIFO, O_RDWR);
	if (s_fifo_fd == -1)
	{
		perror("\nErro ao abrir o FIFO do servidor (RDWR/blocking)");
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "\nFIFO aberto para READ (+WRITE) bloqueante");

	while (1)
	{
	}

	return EXIT_SUCCESS;
}
