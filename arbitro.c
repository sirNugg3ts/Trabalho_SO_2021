#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

	if (getenv("GAMEDIR") != NULL){
		variaveis.gamedir = getenv("GAMEDIR");
	}else{
		variaveis.gamedir = GAMEDIR;
	}

	if(getenv("MAXPLAYERS") != NULL){
		variaveis.maxplayers = atoi(getenv("MAXPLAYERS"));
	}else{
		variaveis.maxplayers = MAXPLAYERS;
	}


}
