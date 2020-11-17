#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"

int main(int argc, char **argv){
	PLAYER jogador;

	//obter identificação do jogador
	printf("\nInsira o seu nome:");
	scanf("%s",jogador.nome);
	printf("\nNome recebido:%s",jogador.nome);
	return EXIT_SUCCESS;
}
