#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include  "arbitro.h"

int main(int argc, char **argv){

	/*
	+ -> done
	- -> to be done
	* -> not finished

	+Lógica de leitura das variáveis de ambiente
	*Constantes simbólicas que registem os valores por omissão
	-obter a duracação do campeonato e o tempo de espera

	*/

	VARS variaveis;
	variaveis.duracaoCampeonato = -1;
	variaveis.tempoEspera = -1;

	//Obtenção das variáveis de ambiente
	if (getenv("GAMEDIR") != NULL){
		variaveis.gamedir = getenv("GAMEDIR");
	}else{
		variaveis.gamedir = GAMEDIR;
	}

	if(getenv("MAXPLAYERS") != NULL){
		if(atoi(getenv("MAXPLAYERS"))> 30){
			printf("\nValor invalido definido na variavel de ambiente MAXPLAYERS, voltando para default -> %d",MAXPLAYERS);
			variaveis.maxplayers = MAXPLAYERS;
		}else{
			variaveis.maxplayers = atoi(getenv("MAXPLAYERS"));
		}
	}else{
		printf("\nVariavel de ambiente MAXPLAYERS nao definida, going default -> %d",MAXPLAYERS);
		variaveis.maxplayers = MAXPLAYERS;
	}

	printf("\nDiretorio do gamedir: %s",variaveis.gamedir);
	printf("\nMaxPlayers: %d",variaveis.maxplayers);

	//obtenção da duração do campeonato e tempo de espera

	int indice;

	while ((indice = getopt(argc,argv,"c:t:"))!= -1) //usage: ./arbitro -c 60000 -t 10000
	{
		switch (indice)
		{
		case 'c':
			variaveis.duracaoCampeonato = atoi(optarg);
			printf("\nDuracao campeonato recebida: %d",variaveis.duracaoCampeonato);
			break;

		case 't':
			variaveis.tempoEspera = atoi(optarg);
			printf("\nTempo de espera recebido: %d",variaveis.tempoEspera);
			break;
		default:
			printf("\nArgumento inválido");
		break;
		}
	}

	//caso não receba o valor na linha de comandos fica com estes valores
	if (variaveis.tempoEspera == -1)
	{
		printf("\nNao foi recebido nenhum valor para tempo de espera, defaulting to %d",TEMPOESPERA);
		variaveis.tempoEspera = TEMPOESPERA;
	}
	if (variaveis.duracaoCampeonato == -1)
	{
		printf("\nNao foi recebido nenhum valor para a duracao do campeonato, defaulting to %d",DURACAOCAMPEONATO);
		variaveis.duracaoCampeonato = DURACAOCAMPEONATO;
	}

	printf("\ngamedir:%s\nmaxplayers:%d\ntempo espera:%d\nduracao campeonato:%d",variaveis.gamedir,variaveis.maxplayers,variaveis.tempoEspera,variaveis.duracaoCampeonato);
	
	
	


}
