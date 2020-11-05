#include <stdio.h>
#include "cliente.h"

int main(){
	PLAYER jogador;

	//obter identificação do jogador
	printf("\nInsira o seu nome:");
	scanf("%s",jogador.nome);
	printf("\nNome recebido:%s",jogador.nome);
}
