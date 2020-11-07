#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogo.h"

int main(int argc, char **argv) {

   JOGO jogo;
   jogo.duracao = DEFAULTDURACAO;

int moeda, cara = 0, coroa = 0,pontuacao = 0, campeonato = 0;
char escolha[10];
time_t t;
srand((unsigned) time(&t));

printf("\n------------ Coin Flip ------------\n");
printf("O jogo consiste em atirar a moeda ao ar.\n");
printf("O jogador escolhe entre Cara ou Coroa.\n");
printf("Caso o jogador acerte, o jogador pontua 1000 pontos. Caso erre perde 500 pontos.");
printf("\n-------------------------------\n");
do{
  do{
  printf("Cara ou Coroa?\n");
  scanf("%s", escolha);
  }while(strcmp(escolha, "Coroa") != 0 && strcmp(escolha, "Cara") != 0);
  for(int  n = 0 ; n < 3 ; n++ ) {
  moeda = rand();
  }
 if (moeda%2 == 0){
     printf("Saiu Cara\n");
 }
 else{
     printf("Saiu Coroa\n");
 }
  if (moeda%2 == 0 && strcmp(escolha, "Cara") == 0 ){
     printf("Parabens! Pontuou.\n");
     pontuacao = pontuacao + 1000;
  }
  else if (moeda%2 != 0 && strcmp(escolha, "Coroa") == 0 ){
     printf("Parabens! Pontuou.\n");
     pontuacao = pontuacao + 1000;
  }else{
     printf("Errou!\n");
     pontuacao = pontuacao - 500;
  }
     campeonato++;
  printf("-------------------------------\n");
}while(campeonato<4); // este ciclo ser� substituido por um ciclo que far� o jogo terminar qnd o campeonato terminar
  printf("\nO jogador pontuou %d pontos!", pontuacao);
  printf("\n-------------------------------\n");
return pontuacao
}
