#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "jogo.h"

void sig_handler(int SIG){
   if (SIG == SIGURS1){
      printf("\nComando para terminar recebido\nA dar exit com o pontuacao");
      exit(jogo.pontuacao);
   }
     
}

int main(int argc, char **argv) {

   GAME jogo;
   jogo.duracao = DEFAULTDURACAO; //enquanto não sabemos quanto tempo tem o jogo

int moeda, campeonato = 0;
char escolha[10];
time_t t;
srand((unsigned) time(&t));

if (signal(SIGURS1,sig_handler))
{
   printf("\nErro no sinal SIGURS1");
}


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
     jogo.pontuacao = jogo.pontuacao + 1000;
  }
  else if (moeda%2 != 0 && strcmp(escolha, "Coroa") == 0 ){
     printf("Parabens! Pontuou.\n");
     jogo.pontuacao = jogo.pontuacao + 1000;
  }else{
     printf("Errou!\n");
     jogo.pontuacao = jogo.pontuacao - 500;
  }
     campeonato++;
  printf("-------------------------------\n");
}while(campeonato<4); // este ciclo ser� substituido por um ciclo que far� o jogo terminar qnd o campeonato terminar
  printf("\nO jogador pontuou %d pontos!", jogo.pontuacao);
  printf("\n-------------------------------\n");
return jogo.pontuacao;
}
