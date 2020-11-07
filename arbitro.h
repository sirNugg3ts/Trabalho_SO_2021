#ifndef ARBITRO
#define ARBITRO

//valores a ser usados caso não sejam recebidos valores ou não estejam definidos
#define MAXPLAYERS 10
#define GAMEDIR "~/gamedir/"
#define DURACAOCAMPEONATO 300000 //ainda não está a ser usado, 5 minutos em ms
#define TEMPOESPERA 60000 //ainda não está a ser usado, 1 minuto em ms

//struct para guardar as variáveis todas
typedef struct VARS{
    int duracaoCampeonato;
    int tempoEspera;
    char *gamedir;
	int maxplayers;
}VARS;

//aquilo que o arbitro precisa de saber do campeonato
typedef struct arbitro{
    int nJogadores;
}infoArbitro;

#endif